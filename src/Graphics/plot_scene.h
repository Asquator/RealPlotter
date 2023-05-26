#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPaintEvent>
#include <QPair>
#include <QGraphicsScene>
#include <QPainter>


class PlotScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PlotScene(QWidget *parent = nullptr);

    void drawAxes();

public slots:
    void zoomed(double newScale);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    const double AXES_WIDTH_TO_PLOT_SIZE = 0.0015;


    double m_plotScale;

    std::pair<double, double> calcAxesWidth(const QRectF& rect);

};

#endif // PLOTCANVAS_H
