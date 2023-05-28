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

    static constexpr double SCENE_SIDE = INT_MAX;
    static const double UNIT_SCALE_SIDE;

public slots:
    void updateScale(double newScale);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    //number of frames to render at each side (invisible at the time of rendering)
    static constexpr int EXTRA_RENDER_OFFSET = 100;

    const double LINES_WIDTH_TO_PLOT_SIZE = 0.0015;

    double gridScale = 1;
    void drawGrid(QPainter *painter, const QRectF &rect);

};

#endif // PLOTCANVAS_H
