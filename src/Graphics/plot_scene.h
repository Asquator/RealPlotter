#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPaintEvent>

#include <QGraphicsScene>
#include <QPainter>


class PlotScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PlotScene(QWidget *parent = nullptr);

    void addAxes();
    double getGridScale(){return gridScale;}

public slots:
    void updateScale(double newScale);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    const double LINES_WIDTH_TO_PLOT_SIZE = 0.0015;

    //number of frames to render at each side (invisible at the time of rendering)
    const double EXTRA_RENDER_FACTOR = 1;

    double gridScale = 1;
    void drawGrid(QPainter *painter, const QRectF &rect);

};

#endif // PLOTCANVAS_H
