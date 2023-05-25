#include "plot_scene.h"

PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}
{
     setSceneRect(-5000,-5000,10000,10000);
     //setSceneRect(-INT_MAX/2,-INT_MAX/2, INT_MAX, INT_MAX);
}

void PlotScene::drawBackground(QPainter *painter, const QRectF &rect){
    setBackgroundBrush(Qt::white);

    //drawing x,y axes
    QGraphicsScene::drawBackground(painter, rect);

    painter->setPen(Qt::black);

    painter->drawLine(-width(), 0, width(), 0);
    painter->drawLine(0, height(), 0, -height());
}
