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
    scene->requestNewCenter(0,0);
    centerOn(scene->sceneRect().center());

    connect(this, SIGNAL(zoomed(double)), scene, SLOT(updateGridUnits(double)));
    connect(scene, &PlotScene::scaleAboutToChange, this, &PlotView::unitRescale);

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &PlotView::horizontalMoved);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &PlotView::verticalMoved);

    zoomScale = 1;
}


QRectF PlotView::visibleRect() const{
    return mapToScene(rect()).boundingRect();
}


QPointF PlotView::visibleCenter() const{
    return visibleRect().center();
}


using std::abs;

void PlotView::horizontalMoved(int newVal)
{
    QScrollBar *bar = static_cast<QScrollBar *>(sender());

}


void PlotView::verticalMoved(int newVal)
{
    QScrollBar *bar = static_cast<QScrollBar *>(sender());
}


void PlotView::unitRescale(double factor){
    PlotScene *pc = static_cast<PlotScene *>(scene());

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
        PlotScene *pc = static_cast<PlotScene *>(scene());
        QRectF rect = visibleRect();

        double currentSide = std::max(rect.width(), rect.height());
        double desiredSide = pc->getUnitScaledSide();

        unitRescale(desiredSide/currentSide);
    }

    firstTime = false;
}

void PlotView::moveCenterHere()
{
    PlotScene *pc = static_cast<PlotScene *>(scene());
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
