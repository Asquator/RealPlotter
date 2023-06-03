#include <float.h>
#include <iostream>
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

    connect(this, SIGNAL(viewScaled(double)), scene, SLOT(scaleCoordinatesFactor(double)));

    centerOn(scene->sceneRect().center());

    unitRescale();
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

    while(abs(currentSide - desiredSide) / desiredSide > allowedScaleError){
        double factor = currentSide / desiredSide;
        QPointF newCenter = rect.center() / factor;

        scale(factor, factor);
        centerOn(newCenter);

        //emit viewScaled(factor);

        rect = visibleRect();

        currentSide = std::max(rect.width(), rect.height());
        desiredSide = static_cast<PlotScene *>(scene())->getUnitScaledSide();
    }

    zoomScale = 1;
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

    double width = visibleRect().width();
    double height = visibleRect().height();

    static_cast<PlotScene *>(scene())->updateGridUnits(zoomScale);

    std::cout << zoomScale << std::endl;
    std::cout << visibleRect().x() <<" " << visibleRect().y() << " " << width << " " << height << std::endl;
}
