#include "plot_scene.h"
#include <float.h>
#include <QGraphicsItem>

PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}{

    setSceneRect(-INT_MAX/2, -INT_MAX/2, INT_MAX, INT_MAX);

    //setSceneRect(-100000000, -100000000, 100000000, 100000000);
    QGraphicsLineItem *x_axis = new QGraphicsLineItem(-width()/2, 0, width()/2, 0);
    QGraphicsLineItem *y_axis = new QGraphicsLineItem(0, height()/2, 0, -height()/2);

    x_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    y_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    addItem(x_axis);
    addItem(y_axis);
}

void PlotScene::drawBackground(QPainter *painter, const QRectF &rect){
    //fill white background
    setBackgroundBrush(Qt::white);
    QGraphicsScene::drawBackground(painter, rect);

    std::pair<double, double> penSizes = calcAxesWidth(rect);

    //draw axes

/*
    QPen pen = QPen(Qt::black);

    pen.setWidth(penSizes.first);
    painter->setPen(pen);
    painter->drawLine(-width(), 0, width(), 0);

    pen.setWidth(penSizes.second);
    painter->setPen(pen);
    painter->drawLine(0, height(), 0, -height());

    //set pen for grid
    pen.setStyle(Qt::DashDotDotLine);
    painter->setPen(pen);
*/

    //draw grid
}

void PlotScene::zoomed(double newScale){


}

std::pair<double, double> PlotScene::calcAxesWidth(const QRectF &rect){
    return std::pair<double, double>{rect.height() * AXES_WIDTH_TO_PLOT_SIZE,
                                 rect.width() * AXES_WIDTH_TO_PLOT_SIZE};

}
