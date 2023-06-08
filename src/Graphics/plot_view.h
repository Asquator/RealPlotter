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

    QRectF visibleRect() const;
    QPointF visibleCenter() const;

public slots:
    void unitRescale(double factor);

    void horizontalMoved(int newVal);
    void verticalMoved(int newVal);


signals:
    void viewChanged();
    void zoomed(double factor);

private:
    double zoomScale = 1;
    const double SCROLL_FACTOR = 0.0005;

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void moveCenterHere();


protected:
    void wheelEvent(QWheelEvent *event);

};

#endif // PLOT_VIEW_H
