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

   // void addAxes();

    //double getGridScale(){return relativeGridScale;}

    static constexpr double SCENE_SIDE = 100000;
    static constexpr int N_DEFAULT_GRID_LINES = 8;

    static const double UNIT_SCALE_SIDE;

    double getUnitScale() const;

    double mapXToRealCoords(double sceneCoordinate);
    double mapYToRealCoords(double sceneCoordinate);

    double mapXToSceneCoords(double realCoordinate);
    double mapYToSceneCoords(double realCoordinate);

    double getUnitScaledSide();

    static constexpr double REQUESTED_SHIFT_COEF = 0.5;

    QPointF getRealCenter() const;

public slots:
    void updateGridUnits(double newViewScale);
    void scaleCoordinatesFactor(double scale);

    void requestNewCenter(QPointF newCenter);

signals:
    void basicUnitUpdated();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    //coords to render at each side (invisible at the time of rendering)
    static constexpr int EXTRA_RENDER_OFFSET = 100;
    static const double MAX_RECOMMENDED_ZOOM;

    const double TEXT_WIDTH_TO_PLOT_SIZE = 0.015;
    const int LINE_WIDTH = 3;

    double gridScale = 1;
    double absoluteScale = 1;
    double relativeScale = 1;

    QPointF realCenter;

    double unitScale = 1; //to be removed

    double coordinateMappingCoef = UNIT_SCALE_SIDE;

    void drawAxes(QPainter *painter);
    void drawGrid(QPainter *painter, const QRectF &rect);

};

#endif // PLOTCANVAS_H
