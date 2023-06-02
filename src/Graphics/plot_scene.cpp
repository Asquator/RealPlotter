#include <float.h>
#include <QGraphicsItem>
#include <iostream>
#include <cmath>
#include <array>

#include "plot_scene.h"


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


using std::fabs; using std::fmod; using std::min; using std::max;

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){
    const QPointF sceneCenter = sceneRect().center();
    QPen pen = QPen(Qt::black);
    pen.setCosmetic(true);

    painter->setPen(pen);
    std::cout << TEXT_WIDTH_TO_PLOT_SIZE * rect.width() << std::endl;
    painter->setFont(QFont("Arial", TEXT_WIDTH_TO_PLOT_SIZE * rect.width(), 500));

    double coordGap = UNIT_SCALE_SIDE / relativeGridScale;
    double realGap = 1 / absoluteZoomScale;

    double top = rect.top(), bottom = rect.bottom(), left = rect.left(), right = rect.right();

    double coordLevel, gridLabel, startUnitCoord;

    //horizontal

    startUnitCoord = round(top / coordGap);
    //iterate from top to bottom and draw lines
    for (coordLevel = coordGap * startUnitCoord, gridLabel = realGap * startUnitCoord;
        coordLevel < bottom + coordGap;
        coordLevel += coordGap, gridLabel += realGap) {
        painter->drawLine(left - EXTRA_RENDER_OFFSET, coordLevel,
                        right + EXTRA_RENDER_OFFSET, coordLevel);

        painter->drawText(sceneCenter.x(), coordLevel, TEXT_WIDTH_TO_PLOT_SIZE * rect.width() * 10,
                                                     TEXT_WIDTH_TO_PLOT_SIZE * rect.height() * 2.5,
                          Qt::AlignVCenter | Qt::AlignVCenter, QString::number(-gridLabel));
    }

    //vertical

    startUnitCoord = round(left / coordGap);
    //iterate from left to right and draw lines
    for (coordLevel = coordGap * startUnitCoord, gridLabel = realGap * startUnitCoord;
        coordLevel <= right + coordGap;
        coordLevel += coordGap, gridLabel += realGap) {
        painter->drawLine(coordLevel, top - EXTRA_RENDER_OFFSET,
                        coordLevel, bottom + EXTRA_RENDER_OFFSET);

        painter->drawText(coordLevel, sceneCenter.y(), TEXT_WIDTH_TO_PLOT_SIZE * rect.width() * 10,
                                                    TEXT_WIDTH_TO_PLOT_SIZE * rect.height() * 2.5,
                          Qt::AlignVCenter | Qt::AlignVCenter, QString::number(gridLabel));
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

/*
void PlotScene::exceededZoomIn(){
    relativeZoomScale = 1;
}

void PlotScene::exceededZoomOut(){
    relativeZoomScale = 1;
}
*/

void PlotScene::updateGridUnits(double newViewScale){
    static CircularScaler<3> scaler({2,2.5,2}, 0);
    bool updated = false;
    double nextScale;

    //unitScale *= newViewScale;

    std::cout << "SCALE: " << relativeGridScale;
    double zoomRatio = newViewScale / relativeGridScale;

    if(zoomRatio >= scaler.nextUp()){ //zoomed in
        nextScale = scaler.scaleUp();
        relativeGridScale *= nextScale;
        absoluteZoomScale *= nextScale;
        unitScale = 1;
    }

    else if(zoomRatio <= 1 / scaler.nextDown()){ //zoomed out
        nextScale = scaler.scaleDown();
        relativeGridScale /= nextScale;
        absoluteZoomScale /= nextScale;
        unitScale = 1;
    }

    if(relativeGridScale >= MAX_RECOMMENDED_ZOOM){
        relativeGridScale = 1;
        emit basicUnitUpdated();
    }

    else if(relativeGridScale <= 1 / MAX_RECOMMENDED_ZOOM){
        relativeGridScale = 1;
        emit basicUnitUpdated();
    }

}
