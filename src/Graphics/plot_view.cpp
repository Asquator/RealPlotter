#include <float.h>

#ifndef NDEBUG
#include <iostream>
#endif

#include <QRectF>
#include <QPolygonF>
#include <algorithm>
#include <cmath>

#include <QScrollBar>

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

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &PlotView::viewChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &PlotView::viewChanged);
}


void PlotView::setScene(PlotScene *scene){
    QGraphicsView::setScene(scene);

    connect(scene, &PlotScene::scaleChanged, this, &PlotView::unitRescale);

    centerOn(scene->sceneRect().center());
    zoomScale = 1;
}


QRectF PlotView::visibleRect(){
    return mapToScene(rect()).boundingRect();
}


using std::abs;


void PlotView::unitRescale(double factor){
    QRectF rect = visibleRect();
    PlotScene *pc = static_cast<PlotScene *>(scene());

    auto anchor = transformationAnchor();
    setTransformationAnchor(ViewportAnchor::AnchorViewCenter);

        QPointF center = rect.center();
        factor = 1 / factor;

        scale(factor, factor);
        QRectF newrect = visibleRect();

        QPointF newCenter = center / factor;

        centerOn(newCenter);

        //emit viewScaled(factor);

        rect = visibleRect();

    setTransformationAnchor(anchor);
}


void PlotView::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsView::drawBackground(painter, rect);

    static bool firstTime = true;

    if(firstTime){
        PlotScene *pc = static_cast<PlotScene *>(scene());
        QRectF rect = visibleRect();

        double currentSide = std::max(rect.width(), rect.height());
        double desiredSide = pc->getUnitScaledSide();

        unitRescale(desiredSide/currentSide);
    }

    firstTime = false;
}


void PlotView::wheelEvent(QWheelEvent *event){
    int angle = event->angleDelta().y();
    double scaleFactor = 1 + angle * SCROLL_FACTOR;

    emit zoomed(zoomScale);
    scale(scaleFactor, scaleFactor);
    zoomScale *= scaleFactor;

    #ifndef NDEBUG
    double width = visibleRect().width(), height = visibleRect().height();
    std::cout << "view zoom scale: " << zoomScale << std::endl;
    std::cout << "visible rectangle: " << visibleRect().x() <<" " << visibleRect().y() << " " << width << " " << height << std::endl;
    std::cout << ((PlotScene *)(scene()))->mapXToSceneCoords(0) << " " << ((PlotScene *)(scene()))->mapYToSceneCoords(0) << std::endl;
#endif
}
