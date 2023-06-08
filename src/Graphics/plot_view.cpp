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

    connect(scene, &PlotScene::basicUnitUpdated, this, [this](){
        unitRescale();
    });

    centerOn(scene->sceneRect().center());
    unitRescale();
    zoomScale = 1;
}


QRectF PlotView::visibleRect(){
    return mapToScene(rect()).boundingRect();
}


using std::abs;


void PlotView::unitRescale(){
    const double allowedScaleError = 0.000001;

    QRectF rect = visibleRect();

    double currentSide = std::max(rect.width(), rect.height());
    double desiredSide = static_cast<PlotScene *>(scene())->getUnitScaledSide();

    auto anchor = transformationAnchor();
    setTransformationAnchor(ViewportAnchor::AnchorViewCenter);

    while(abs(currentSide - desiredSide) / desiredSide > allowedScaleError){
        double factor = currentSide / desiredSide;
        QPointF center = rect.center();

        scale(factor, factor);
        QRectF newrect = visibleRect();

        QPointF newCenter = center / factor;

        centerOn(newCenter);

        //emit viewScaled(factor);

        rect = visibleRect();


        currentSide = std::max(rect.width(), rect.height());
        desiredSide = static_cast<PlotScene *>(scene())->getUnitScaledSide();
    }

    setTransformationAnchor(anchor);
}


void PlotView::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsView::drawBackground(painter, rect);

    static bool firstTime = true;

    if(firstTime)
        unitRescale();

    firstTime = false;
}


void PlotView::wheelEvent(QWheelEvent *event){
    int angle = event->angleDelta().y();
    double scaleFactor = 1 + angle * SCROLL_FACTOR;
    scale(scaleFactor, scaleFactor);
    zoomScale *= scaleFactor;

    emit viewChanged();
    emit zoomed(zoomScale);

    #ifndef NDEBUG
    double width = visibleRect().width(), height = visibleRect().height();
    std::cout << "view zoom scale: " << zoomScale << std::endl;
    std::cout << "visible rectangle: " << visibleRect().x() <<" " << visibleRect().y() << " " << width << " " << height << std::endl;
    std::cout << ((PlotScene *)(scene()))->mapXToSceneCoords(0) << " " << ((PlotScene *)(scene()))->mapYToSceneCoords(0) << std::endl;
#endif
}
