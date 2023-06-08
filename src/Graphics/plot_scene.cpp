#include <float.h>

#ifndef NDEBUG
#include <iostream>
#endif

#include <cmath>
#include <array>
#include <vector>
#include <future>

#include <QGraphicsItem>

#include <RealFunctionLib/real_function.h>

#include "plot_scene.h"
#include "function_list_model.h"


PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}{

    setSceneRect(-SCENE_SIDE/2, -SCENE_SIDE/2, SCENE_SIDE, SCENE_SIDE);


    //addAxes();
}


const double PlotScene::UNIT_SCALE_SIDE = sqrt(SCENE_SIDE);
const double PlotScene::MAX_RECOMMENDED_ZOOM = 0.01 * UNIT_SCALE_SIDE;

/*
void PlotScene::addAxes(){
    QPen axesPen(Qt::black);
    axesPen.setWidth(3);

    QPointF sceneCenter = sceneRect().center();

    x_axis = new QGraphicsLineItem(-width()/2 + realCenter.x(), sceneCenter.y() + realCenter.y(),
                                   width()/2 + realCenter.x(), sceneCenter.y() + realCenter.y());

    y_axis = new QGraphicsLineItem(sceneCenter.x() + realCenter.x(), height()/2 + realCenter.y(),
                                   sceneCenter.x() + realCenter.x(), -height()/2 + realCenter.y());

    x_axis->setPen(axesPen);
    y_axis->setPen(axesPen);

    x_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    y_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    addItem(x_axis);
    addItem(y_axis);
}
*/

void PlotScene::drawBackground(QPainter *painter, const QRectF &rect){
    //fill white background
    setBackgroundBrush(Qt::white);
    QGraphicsScene::drawBackground(painter, rect);

    drawAxes(painter);

    //draw dynamic grid dependent on the rect position
    drawGrid(painter, rect);
}

QPointF PlotScene::getRealCenter() const
{
    return realCenter;
}


double PlotScene::mapXToRealCoords(double crd)
{
    return crd / UNIT_SCALE_SIDE / gridScale + realCenter.x();
}


double PlotScene::mapYToRealCoords(double crd)
{
    return realCenter.y() - crd / UNIT_SCALE_SIDE / gridScale;
}


double PlotScene::mapXToSceneCoords(double crd)
{
    return (crd - realCenter.x()) * UNIT_SCALE_SIDE * gridScale;
}


double PlotScene::mapYToSceneCoords(double crd)
{
    return (realCenter.y() - crd) * UNIT_SCALE_SIDE * gridScale;
}


QPointF PlotScene::mapToSceneCoords(const QPointF &point)
{
    return QPointF(mapXToSceneCoords(point.x()), mapYToSceneCoords(point.y()));
}


QPointF PlotScene::mapToRealCoords(const QPointF &point)
{
    return QPointF(mapXToRealCoords(point.x()), mapYToRealCoords(point.y()));
}



double PlotScene::getUnitScaledSide()
{
    return UNIT_SCALE_SIDE * N_DEFAULT_GRID_LINES;
}


QPointF PlotScene::getOriginInSceneCoords()
{
    return QPointF{mapXToSceneCoords(0), mapYToSceneCoords(0)};
}


using std::fabs; using std::fmod; using std::min; using std::max;

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){
    const QPointF sceneCenter = sceneRect().center();
    QPen pen = QPen(Qt::black);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setFont(QFont("Arial", TEXT_WIDTH_TO_PLOT_SIZE * rect.width(), 500));

    const double coordGap = UNIT_SCALE_SIDE;
    const double realGap = 1 / gridScale;
    const double top = rect.top(), bottom = rect.bottom(), left = rect.left(), right = rect.right();
    const double textWidth = TEXT_WIDTH_TO_PLOT_SIZE * rect.width() * 8,
        textHeight = TEXT_WIDTH_TO_PLOT_SIZE * rect.height() * 2;

    double coordLevel, gridLabel, startUnitCoord;
    int labelFlags = 0;

    #ifndef NDEBUG
    //std::cout << "grid scale " << gridScale << std::endl;
    #endif

    QPointF origin = getOriginInSceneCoords();

    //horizontal
    startUnitCoord = round((top - origin.y()) / coordGap);
    //iterate from top to bottom and draw lines
    for (coordLevel = coordGap * startUnitCoord + origin.y(), gridLabel = realGap * startUnitCoord;
         coordLevel < bottom + coordGap;
         coordLevel += coordGap, gridLabel += realGap) {
        painter->drawLine(left - EXTRA_RENDER_OFFSET, coordLevel,
                          right + EXTRA_RENDER_OFFSET, coordLevel);

        if(sceneCenter.x() > right)
            labelFlags |= Qt::AlignRight;

        else
            labelFlags |= Qt::AlignLeft;

        painter->drawText(min(max(origin.x(), left), right - textWidth), coordLevel, textWidth, textHeight,
                          labelFlags, QString::number(-gridLabel));
    }

    //vertical

    startUnitCoord = round((left - origin.x())/ coordGap);
    //iterate from left to right and draw lines
    for (coordLevel = coordGap * startUnitCoord + origin.x(), gridLabel = realGap * startUnitCoord;
         coordLevel <= right + coordGap;
         coordLevel += coordGap, gridLabel += realGap) {
        painter->drawLine(coordLevel, top - EXTRA_RENDER_OFFSET,
                          coordLevel, bottom + EXTRA_RENDER_OFFSET);

        painter->drawText(coordLevel, min(max(origin.y(), top), bottom - textHeight), textWidth, textHeight,
                          Qt::AlignLeft, QString::number(gridLabel));
    }
}

void PlotScene::drawAxes(QPainter *painter)
{
    QPen pen = QPen(Qt::black);
    double w = 3 * LINE_WIDTH;
    pen.setWidth(w);

    painter->setPen(pen);

    //x axis
    double coord = mapYToSceneCoords(0);
    painter->drawLine(-SCENE_SIDE, coord, SCENE_SIDE, coord);
    //std::cout <<"x "<< coord << std::endl;
    //y axis
    coord = mapXToSceneCoords(0);
    painter->drawLine(coord, -SCENE_SIDE, coord, SCENE_SIDE);
    //std::cout << "y "<< coord << std::endl;
}


template <int N> class CircularScaler {
public:
    CircularScaler(std::array<double, N> arr, int startUpscalePos):
        factors(arr), pos{startUpscalePos} {}

    double nextUp(){
        return factors[pos];
    }

    double nextDown(){
        return pos == 0 ? factors[factors.size() - 1] : factors[pos];
    }

    double scaleUp(){
        //take this element as a scale factor
        double ret = factors[pos++];
        if(pos == factors.size())
            pos = 0;

        return ret;
    }

    double scaleDown(){
        //take previous value as a scale factor
        if(--pos < 0)
            pos = factors.size()-1;

        return factors[pos];
    }

private:
    int pos;
    std::array<double, N> factors;
};


void PlotScene::updateGridUnits(double newViewScale){
    static CircularScaler<3> scaler({2, 2.5, 2}, 0);
    bool updated = false;
    double nextScale;
    double zoomRatio = newViewScale / gridScale;

    #ifndef NDEBUG
        std::cout << "NEW VIEW SCALE: " << newViewScale
                  << " GRID SCALE :" << gridScale <<
                " RELATIVE SCALE: " << relativeScale <<  std::endl;
    #endif

    if(zoomRatio >= scaler.nextUp()){ //zoomed in
        nextScale = scaler.scaleUp();
        emit scaleAboutToChange(nextScale);
        gridScale *= nextScale;
        relativeScale = 1;

    }

    else if(zoomRatio <= 1 / scaler.nextDown()){ //zoomed out
        nextScale = scaler.scaleDown();
        emit scaleAboutToChange(1 / nextScale);
        gridScale /= nextScale;
        relativeScale = 1;

    }
}

void PlotScene::requestNewCenter(const QPointF &center)
{
    realCenter = center;
}


void PlotScene::requestNewCenter(double x, double y)
{
    realCenter = QPointF{x,y};
}

