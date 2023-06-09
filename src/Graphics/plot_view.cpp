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
}


void PlotView::setScene(PlotScene *scene){
    QGraphicsView::setScene(scene);
    scene->requestNewCenter(0,0);
    centerOn(scene->sceneRect().center());

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &PlotView::scrollbarMoved);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &PlotView::scrollbarMoved);

    connect(this, SIGNAL(zoomed(double)), scene, SLOT(updateGridUnits(double)));
    connect(scene, &PlotScene::scaleAboutToChange, this, &PlotView::unitRescale);
}


PlotScene *PlotView::scene()
{
    return static_cast<PlotScene *>(QGraphicsView::scene());
}


QRectF PlotView::visibleRect() const{
    return mapToScene(rect()).boundingRect();
}


QPointF PlotView::visibleCenter() const{
    return visibleRect().center();
}


using std::abs;

void PlotView::scrollbarMoved(int newVal)
{
    //scrollbar coefficient range in which center remains static
    constexpr double static_scroll_range = 0.9;

    QScrollBar *bar = static_cast<QScrollBar *>(sender());
    if(newVal >= static_scroll_range * bar->maximum() || newVal <= (1- static_scroll_range) * bar->minimum())
        moveCenterHere();

    #ifndef NDEBUG
    std::cout << "horizontal: " << horizontalScrollBar()->value() << " vertical: " << verticalScrollBar()->value() << std::endl;
    #endif
}


void PlotView::unitRescale(double factor){
    auto anchor = transformationAnchor();
    setTransformationAnchor(ViewportAnchor::AnchorViewCenter);

    moveCenterHere();

    factor = 1 / factor;
    scale(factor, factor);

    setTransformationAnchor(anchor);
}


void PlotView::drawBackground(QPainter *painter, const QRectF &rect){
    QGraphicsView::drawBackground(painter, rect);

    static bool firstTime = true;

    if(firstTime){
        QRectF rect = visibleRect();

        double currentSide = std::max(rect.width(), rect.height());
        double desiredSide = scene()->getUnitScaledSide();

        unitRescale(desiredSide/currentSide);
    }

    firstTime = false;
}

void PlotView::moveCenterHere()
{
    PlotScene *pc = scene();
    pc->requestNewCenter(pc->mapToRealCoords(visibleCenter()));
    centerOn(pc->sceneRect().center());
}


void PlotView::wheelEvent(QWheelEvent *event){
    int angle = event->angleDelta().y();
    double scaleFactor = 1 + angle * SCROLL_FACTOR;

    scale(scaleFactor, scaleFactor);
    zoomScale *= scaleFactor;

    emit zoomed(zoomScale);

    #ifndef NDEBUG
    double width = visibleRect().width(), height = visibleRect().height();
    std::cout << "view zoom scale: " << zoomScale << std::endl;
    std::cout << "visible rectangle: " << visibleRect().x() <<" " << visibleRect().y() << " " << width << " " << height << std::endl;
    #endif
}
