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
    ~PlotScene();

    void createAxes();

    static constexpr double SCENE_SIDE = 100000;
    static constexpr int N_DEFAULT_GRID_LINES = 10;

    static const double UNIT_COORD_LENGTH;

    double mapXToRealCoords(double sceneCoordinate);
    double mapYToRealCoords(double sceneCoordinate);

    double mapXToSceneCoords(double realCoordinate);
    double mapYToSceneCoords(double realCoordinate);

    QPointF mapToSceneCoords(const QPointF &point);
    QPointF mapToRealCoords(const QPointF &point);

    double getUnitScaledSide();

    static constexpr double REQUESTED_SHIFT_COEF = 0.5;

    QPointF getRealCenter() const;
    QPointF getOriginInSceneCoords();


public slots:
    void updateGridUnits(double newViewScale);
    void requestNewCenter(const QPointF &center);
    void requestNewCenter(double x, double y);

signals:
    void scaleAboutToChange(double nextFactor);
    void scaleChanged(double appliedFactor);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    //coords to render at each side (invisible at the time of rendering)
    static constexpr int EXTRA_RENDER_OFFSET = 100;
    static const double MAX_RECOMMENDED_ZOOM;

    const double TEXT_WIDTH_TO_PLOT_SIZE = 0.015;
    const int LINE_WIDTH = 3;

    double gridScale = 1;

    QPointF realCenter {0,0};

    QGraphicsLineItem *x_axis;
    QGraphicsLineItem *y_axis;

    void centerAxes();

    void drawGrid(QPainter *painter, const QRectF &rect);

};

#endif // PLOTCANVAS_H
