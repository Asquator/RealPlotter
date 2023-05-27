#include <float.h>
#include <QGraphicsItem>
#include <iostream>
#include <cmath>

#include "plot_scene.h"


PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}{

    setSceneRect(-INT_MAX/2, -INT_MAX/2, INT_MAX, INT_MAX);

    addAxes();
}


void PlotScene::addAxes(){
    QPen axesPen(Qt::black);
    axesPen.setWidth(2);

    QGraphicsLineItem *x_axis = new QGraphicsLineItem(-width()/2, 0, width()/2, 0);
    QGraphicsLineItem *y_axis = new QGraphicsLineItem(0, height()/2, 0, -height()/2);

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

  //  std::pair<double, double> penSizes = calcAxesWidth(rect);

    /*
    //draw axes
    QPen pen = QPen(Qt::black);

    pen.setWidth(penSizes.first);
    painter->setPen(pen);
    painter->drawLine(-width(), 0, width(), 0);

    pen.setWidth(penSizes.second);
    painter->setPen(pen);
    painter->drawLine(0, height(), 0, -height());

*/
    drawGrid(painter, rect);
}


using std::fabs; using std::fmod;

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){

    QPen pen = QPen(Qt::black);
    //pen.setStyle(Qt::DashDotLine);

    double gap = 1 / gridScale, level, extraRender;
    double top = rect.top(), bottom = rect.bottom(), left = rect.left(), right = rect.right();

    //horizontal
    pen.setWidth(rect.height() * LINES_WIDTH_TO_PLOT_SIZE);
    painter->setPen(pen);
    extraRender = (right - left) * EXTRA_RENDER_FACTOR;

    //iterate from top to bottom and draw lines
    for (level = gap * round(top / gap) - gap; level < bottom + gap; level += gap) {
        painter->drawLine(left - extraRender, level,
                        right + extraRender, level);
    }

    //vertical
    pen.setWidth(rect.width() * LINES_WIDTH_TO_PLOT_SIZE);
    painter->setPen(pen);
    extraRender = (bottom - top) * EXTRA_RENDER_FACTOR;

    //iterate from left to right and draw lines
    for (level = gap * round(left / gap) - gap; level <= right + gap; level += gap) {
        painter->drawLine(level, top - extraRender,
                        level, bottom + extraRender);
    }


}

void PlotScene::updateScale(double newViewScale){
   // static ;

    double zoomRatio = newViewScale / gridScale;

    if(zoomRatio >= 2) //zoomed in
        ;
    else if(zoomRatio <= 0.5) //zoomed out
        ;

}
