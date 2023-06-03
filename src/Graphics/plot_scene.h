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
    double getGridScale(){return relativeGridScale;}

    static constexpr double SCENE_SIDE = 10000000;
    static constexpr int N_DEFAULT_GRID_LINES = 15;

    static const double UNIT_SCALE_SIDE;

    double getUnitScale() const;

public slots:
    void updateGridUnits(double newViewScale);

signals:
    void basicUnitUpdated();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    //number of frames to render at each side (invisible at the time of rendering)
    static constexpr int EXTRA_RENDER_OFFSET = 100;
    static const double MAX_RECOMMENDED_ZOOM;

    const double TEXT_WIDTH_TO_PLOT_SIZE = 0.015;

    double absoluteZoomScale = 1;
    double relativeGridScale = 1;
    double unitScale = 1;

    void drawGrid(QPainter *painter, const QRectF &rect);
};

#endif // PLOTCANVAS_H
