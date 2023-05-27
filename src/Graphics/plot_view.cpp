#include "plot_view.h"
#include <float.h>
#include <iostream>
#include <QRectF>
#include <QPolygonF>

PlotView::PlotView(QWidget *parent) : QGraphicsView(parent)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //start centered on the origin
    setTransformationAnchor(ViewportAnchor::AnchorUnderMouse);

    //disable the scrollers
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void PlotView::setScene(PlotScene *scene){
    QGraphicsView::setScene(scene);
    connect(this, SIGNAL(zoomScaleChanged(double)), scene, SLOT(zoomed(double)));

    QRectF rect = scene->sceneRect();

    fitInView(QRectF(QPointF(-10,-10), QPointF(10,10)));
    centerOn((rect.left() + rect.right()) / 2, (rect.bottom() + rect.top()) / 2);
}

void PlotView::wheelEvent(QWheelEvent *event){
    int angle = event->angleDelta().y();
    double scaleFactor = 1 + angle / 400.0;
    scale(scaleFactor, scaleFactor);
    m_zoomScale *= scaleFactor;

    emit zoomScaleChanged(m_zoomScale);

    std::cout << m_zoomScale << std::endl;
    //std::cout << mapToScene(viewport()->geometry()).boundingRect().x() << " " <<
     //   mapToScene(viewport()->geometry()).boundingRect().y() << std::endl;

    std::cout << mapToScene(rect()).boundingRect().x() <<" " << mapToScene(rect()).boundingRect().y() << std::endl;

}
