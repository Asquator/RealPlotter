#include "plot_scene.h"
#include <float.h>
#include <QGraphicsItem>

#include <iostream>

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

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){

    QPen pen = QPen(Qt::black);
    pen.setStyle(Qt::DashDotLine);

    double gap, level;

    //horizontal
    pen.setWidth(rect.height() * LINES_WIDTH_TO_PLOT_SIZE);
    painter->setPen(pen);

    gap = gridScale / rect.height();
    for (level = rect.top(); level < rect.bottom(); level += 10 * gap) {
        painter->drawLine(rect.left() - EXTRA_RENDER_OFFSET, level,
                          rect.right() + EXTRA_RENDER_OFFSET, level);
    }

    //vertical
    pen.setWidth(rect.width() * LINES_WIDTH_TO_PLOT_SIZE);
    painter->setPen(pen);

    gap = gridScale / rect.width();
    for (level = rect.left(); level < rect.right(); level += 10 * gap) {
        painter->drawLine(level, rect.top() - EXTRA_RENDER_OFFSET,
                          level, rect.bottom() + EXTRA_RENDER_OFFSET);
    }

}

void PlotScene::zoomed(double newViewScale){
    double zoomRatio = newViewScale / gridScale;

    if(zoomRatio >= 2)
        ;
    else if(zoomRatio <= 0.5)
        ;

}
