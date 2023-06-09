#include <float.h>

#ifndef NDEBUG
#include <iostream>
#endif

#include <cmath>
#include <vector>
#include <future>

#include <QGraphicsItem>

#include <RealFunctionLib/real_function.h>

#include "plot_scene.h"
#include "function_list_model.h"

#include "Tools/circular_scaler.h"
#include "Tools/plot_calculations.h"


PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}{

    setSceneRect(-SCENE_SIDE/2, -SCENE_SIDE/2, SCENE_SIDE, SCENE_SIDE);

    createAxes();
}

PlotScene::~PlotScene()
{
    delete x_axis;
    delete y_axis;
}


const double PlotScene::UNIT_COORD_LENGTH = sqrt(SCENE_SIDE);
const double PlotScene::MAX_RECOMMENDED_ZOOM = 0.01 * UNIT_COORD_LENGTH;


void PlotScene::createAxes(){
    QPen axesPen(Qt::black);
    axesPen.setWidth(3);

    QPointF sceneCenter = sceneRect().center();

    x_axis = new QGraphicsLineItem(INT_MIN/2 + realCenter.x(), sceneCenter.y() + realCenter.y(),
                                   INT_MAX/2 + realCenter.x(), sceneCenter.y() + realCenter.y());

    y_axis = new QGraphicsLineItem(sceneCenter.x() + realCenter.x(), INT_MIN/2 + realCenter.y(),
                                   sceneCenter.x() + realCenter.x(), INT_MAX/2 + realCenter.y());

    x_axis->setPen(axesPen);
    y_axis->setPen(axesPen);

    x_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    y_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    addItem(x_axis);
    addItem(y_axis);
}


void PlotScene::drawBackground(QPainter *painter, const QRectF &rect){
    //fill white background
    setBackgroundBrush(Qt::white);
    QGraphicsScene::drawBackground(painter, rect);

    //draw dynamic grid dependent on the rect position
    drawGrid(painter, rect);
}

void PlotScene::centerAxes()
{
    QPointF origin{getOriginInSceneCoords()};
    x_axis->setPos(origin.x(), origin.y());
    y_axis->setPos(origin.x(), origin.y());
}

QPointF PlotScene::getRealCenter() const
{
    return realCenter;
}


double PlotScene::mapXToRealCoords(double crd)
{
    return crd / UNIT_COORD_LENGTH / gridScale + realCenter.x();
}


double PlotScene::mapYToRealCoords(double crd)
{
    return realCenter.y() - crd / UNIT_COORD_LENGTH / gridScale;
}


double PlotScene::mapXToSceneCoords(double crd)
{
    return (crd - realCenter.x()) * UNIT_COORD_LENGTH * gridScale;
}


double PlotScene::mapYToSceneCoords(double crd)
{
    return (realCenter.y() - crd) * UNIT_COORD_LENGTH * gridScale;
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
    return UNIT_COORD_LENGTH * N_DEFAULT_GRID_LINES;
}


QPointF PlotScene::getOriginInSceneCoords()
{
    return QPointF{mapXToSceneCoords(0), mapYToSceneCoords(0)};
}


using std::fabs; using std::fmod; using std::min; using std::max;

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){
    QPen pen = QPen(Qt::black);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setFont(QFont("Arial", TEXT_WIDTH_TO_PLOT_SIZE * rect.width(), 500));

    const double realGap = 1 / gridScale;
    const double top = rect.top(), bottom = rect.bottom(), left = rect.left(), right = rect.right();
    const double textWidth = TEXT_WIDTH_TO_PLOT_SIZE * rect.width() * 8,
        textHeight = TEXT_WIDTH_TO_PLOT_SIZE * rect.height() * 2;

    const double epsilon = Real_Math::real_epsilon * realGap;
    double coordLevel, gridLabel, startUnitCoord;
    int labelFlags = 0;

    const QPointF origin = getOriginInSceneCoords();

    //horizontal

    startUnitCoord = round((top - origin.y()) / UNIT_COORD_LENGTH);
    //iterate from top to bottom and draw lines
    for (coordLevel = UNIT_COORD_LENGTH * startUnitCoord + origin.y(), gridLabel = realGap * startUnitCoord;
         coordLevel < bottom + UNIT_COORD_LENGTH;
         coordLevel += UNIT_COORD_LENGTH, gridLabel += realGap) {
        painter->drawLine(left - EXTRA_RENDER_OFFSET, coordLevel,
                          right + EXTRA_RENDER_OFFSET, coordLevel);

        labelFlags |= origin.x() > right ? Qt::AlignRight : Qt::AlignLeft;

        painter->drawText(min(max(origin.x(), left), right - (origin.x() > right ? textWidth : 0)),
                          coordLevel, textWidth, textHeight, labelFlags, QString::number(-round_zero(gridLabel, epsilon)));
    }

    //vertical

    startUnitCoord = round((left - origin.x()) / UNIT_COORD_LENGTH);
    //iterate from left to right and draw lines
    for (coordLevel = UNIT_COORD_LENGTH * startUnitCoord + origin.x(), gridLabel = realGap * startUnitCoord;
         coordLevel <= right + UNIT_COORD_LENGTH;
         coordLevel += UNIT_COORD_LENGTH, gridLabel += realGap) {
        painter->drawLine(coordLevel, top - EXTRA_RENDER_OFFSET,
                          coordLevel, bottom + EXTRA_RENDER_OFFSET);

        painter->drawText(coordLevel, min(max(origin.y(), top), bottom - textHeight), textWidth, textHeight,
                          Qt::AlignLeft, QString::number(round_zero(gridLabel, epsilon)));
    }
}


void PlotScene::updateGridUnits(double newViewScale){
    static CircularScaler<3> scaler({2, 2.5, 2}, 0);
    bool updated = false;
    double nextScale;
    double zoomRatio = newViewScale / gridScale;

    #ifndef NDEBUG
        std::cout << "NEW VIEW SCALE: " << newViewScale
                  << " GRID SCALE :" << gridScale <<  std::endl;
    #endif

    if(zoomRatio >= scaler.nextUp()){ //zoomed in
        nextScale = scaler.scaleUp();
        emit scaleAboutToChange(nextScale);
        gridScale *= nextScale;

        centerAxes();
        emit scaleChanged(nextScale);
    }

    else if(zoomRatio <= 1 / scaler.nextDown()){ //zoomed out
        nextScale = scaler.scaleDown();
        emit scaleAboutToChange(1 / nextScale);
        gridScale /= nextScale;

        centerAxes();
        emit scaleChanged(nextScale);
    }
}

void PlotScene::requestNewCenter(const QPointF &center)
{
    realCenter = center;
    centerAxes();
}


void PlotScene::requestNewCenter(double x, double y)
{
    requestNewCenter(QPointF{x,y});
}

