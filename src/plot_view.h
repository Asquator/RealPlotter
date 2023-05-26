#ifndef PLOT_VIEW_H
#define PLOT_VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "plot_scene.h"

class PlotView : public QGraphicsView
{
    Q_OBJECT

public:
    PlotView(QWidget *parent);
    void setScene(PlotScene *scene);

signals:
    void zoomScaleChanged(double);

private:
    double m_zoomScale = 1;


protected:
    void wheelEvent(QWheelEvent *event);

};

#endif // PLOT_VIEW_H
