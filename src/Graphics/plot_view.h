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
    void viewChanged();

private:
    double zoomScale = 1;

    const double SCROLL_FACTOR = 0.001;

    void unitRescale();

    void drawBackground(QPainter *painter, const QRectF &rect) override;

protected:
    void wheelEvent(QWheelEvent *event);

};

#endif // PLOT_VIEW_H
