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

    addAxes();
}

const double PlotScene::UNIT_SCALE_SIDE = sqrt(SCENE_SIDE);
const double PlotScene::MAX_RECOMMENDED_ZOOM = 0.01 * UNIT_SCALE_SIDE;

void PlotScene::addAxes(){
    QPen axesPen(Qt::black);
    axesPen.setWidth(3);

    QPointF center = sceneRect().center();

    QGraphicsLineItem *x_axis = new QGraphicsLineItem(-width()/2, center.y(), width()/2, center.y());
    QGraphicsLineItem *y_axis = new QGraphicsLineItem(center.x(), height()/2, center.x(), -height()/2);

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

double PlotScene::getUnitScale() const
{
    return unitScale;
}

double PlotScene::mapToRealCoords(double crd, Axis ax)
{
    return (ax == Axis::Y ? -1 : 1) * crd / coordinateMappingCoef;
}

double PlotScene::mapToSceneCoords(double crd, Axis ax)
{
    return (ax == Axis::Y ? -1 : 1) * crd * coordinateMappingCoef;
}

double PlotScene::getUnitScaledSide()
{
    return UNIT_SCALE_SIDE * N_DEFAULT_GRID_LINES * getUnitScale();
}


using std::fabs; using std::fmod; using std::min; using std::max;

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){
    const QPointF sceneCenter = sceneRect().center();
    QPen pen = QPen(Qt::black);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setFont(QFont("Arial", TEXT_WIDTH_TO_PLOT_SIZE * rect.width(), 500));

    const double coordGap = UNIT_SCALE_SIDE / relativeGridScale;
    const double realGap = 1 / absoluteGridScale;
    const double top = rect.top(), bottom = rect.bottom(), left = rect.left(), right = rect.right();
    const double textWidth = TEXT_WIDTH_TO_PLOT_SIZE * rect.width() * 8,
                textHeight = TEXT_WIDTH_TO_PLOT_SIZE * rect.height() * 2;

    double coordLevel, gridLabel, startUnitCoord;
    int labelFlags = 0;

    //horizontal

    startUnitCoord = round(top / coordGap);
    //iterate from top to bottom and draw lines
    for (coordLevel = coordGap * startUnitCoord, gridLabel = realGap * startUnitCoord;
        coordLevel < bottom + coordGap;
        coordLevel += coordGap, gridLabel += realGap) {
        painter->drawLine(left - EXTRA_RENDER_OFFSET, coordLevel,
                        right + EXTRA_RENDER_OFFSET, coordLevel);

        if(sceneCenter.x() > right)
                labelFlags |= Qt::AlignRight;

        else
                labelFlags |= Qt::AlignLeft;

        painter->drawText(min(max(sceneCenter.x(), left), right - textWidth), coordLevel, textWidth, textHeight,
                          labelFlags, QString::number(-gridLabel));
    }

    //vertical

    startUnitCoord = round(left / coordGap);
    //iterate from left to right and draw lines
    for (coordLevel = coordGap * startUnitCoord, gridLabel = realGap * startUnitCoord;
        coordLevel <= right + coordGap;
        coordLevel += coordGap, gridLabel += realGap) {
        painter->drawLine(coordLevel, top - EXTRA_RENDER_OFFSET,
                        coordLevel, bottom + EXTRA_RENDER_OFFSET);

        painter->drawText(coordLevel, min(max(sceneCenter.y(), top), bottom - textHeight), textWidth, textHeight,
                          Qt::AlignLeft, QString::number(gridLabel));
    }

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
    static CircularScaler<3> scaler({2,2.5,2}, 0);
    bool updated = false;
    double nextScale;

    #ifndef NDEBUG
    std::cout << "UNIT: " << UNIT_SCALE_SIDE << " RELATIVE SCALE: " << relativeGridScale <<
                                                " ABSOLUTE SCALE :" << absoluteGridScale <<
                                                " UNIT SCALE: " << unitScale <<
                                                std::endl;
    #endif

    double zoomRatio = newViewScale / relativeGridScale;

    unitScale *= zoomRatio;

    if(zoomRatio >= scaler.nextUp()){ //zoomed in
        nextScale = scaler.scaleUp();
        relativeGridScale *= nextScale;
        absoluteGridScale *= nextScale;
        unitGridScale *= nextScale;
        unitScale = 1;
    }

    else if(zoomRatio <= 1 / scaler.nextDown()){ //zoomed out
        nextScale = scaler.scaleDown();
        relativeGridScale /= nextScale;
        absoluteGridScale /= nextScale;
        unitGridScale /= nextScale;
        unitScale = 1;
    }

    if(relativeGridScale >= MAX_RECOMMENDED_ZOOM){ //zoomed in exceeding scene size
        relativeGridScale = unitScale;
        double s = unitGridScale;
        coordinateMappingCoef *= s;
        emit basicUnitUpdated();
        unitGridScale = 1;
    }

    else if(relativeGridScale <= 1 / MAX_RECOMMENDED_ZOOM){ //zoomed out exceeding scene size
        relativeGridScale = unitScale;
        double s = absoluteGridScale;
        coordinateMappingCoef *= unitGridScale;
        unitGridScale = 1;
        emit basicUnitUpdated();
    }

}

void PlotScene::scaleCoordinatesFactor(double scale)
{
    coordinateMappingCoef *= scale;
}


using std::vector; using std::future;


