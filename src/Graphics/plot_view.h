#ifndef PLOT_VIEW_H
#define PLOT_VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QRectF>

#include "plot_scene.h"

class PlotView : public QGraphicsView
{
    Q_OBJECT

public:
    PlotView(QWidget *parent);
    void setScene(PlotScene *scene);

    QRectF visibleRect();

signals:
    void zoomScaleChanged(double);

private:
    double zoomScale = 1;

    const double SCROLL_FACTOR = 0.0005; //0.0005;

    void unitRescale();

protected:
    void wheelEvent(QWheelEvent *event);

};

#endif // PLOT_VIEW_H
