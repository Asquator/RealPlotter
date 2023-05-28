#include <float.h>
#include <iostream>
#include <QRectF>
#include <QPolygonF>
#include <algorithm>
#include <cmath>

#include "plot_view.h"


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
    connect(this, SIGNAL(zoomScaleChanged(double)), scene, SLOT(updateScale(double)));

    auto anchorSaved = transformationAnchor();
    setTransformationAnchor(ViewportAnchor::AnchorViewCenter);

    centerOn(scene->sceneRect().center());
    QRectF rect = visibleRect();

    double currentSide = std::max(rect.width(), rect.height());
    double desiredSide = PlotScene::UNIT_SCALE_SIDE;

    double factor = currentSide / desiredSide;
    scale(factor, factor);

    setTransformationAnchor(anchorSaved);
}


QRectF PlotView::visibleRect(){
    return mapToScene(rect()).boundingRect();
}

void PlotView::wheelEvent(QWheelEvent *event){
    int angle = event->angleDelta().y();
    double scaleFactor = 1 + angle * SCROLL_FACTOR;
    scale(scaleFactor, scaleFactor);
    m_zoomScale *= scaleFactor;

    emit zoomScaleChanged(m_zoomScale);

    std::cout << m_zoomScale << std::endl;
    //std::cout << mapToScene(viewport()->geometry()).boundingRect().x() << " " <<
     //   mapToScene(viewport()->geometry()).boundingRect().y() << std::endl;

    std::cout << visibleRect().x() <<" " << visibleRect().y() << " " << visibleRect().width() << " " << visibleRect().height() << std::endl;

}
