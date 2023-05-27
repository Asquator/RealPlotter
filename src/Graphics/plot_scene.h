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
    void drawGrid(QPainter *painter, const QRectF &rect);

public slots:
    void zoomed(double newScale);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    const double LINES_WIDTH_TO_PLOT_SIZE = 0.0015;
    const double EXTRA_RENDER_OFFSET = 100;

    double gridScale = 1;

};

#endif // PLOTCANVAS_H
