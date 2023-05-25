#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPaintEvent>
#include <QRect>
#include <QGraphicsScene>
#include <QPainter>


class PlotScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PlotScene(QWidget *parent = nullptr);

    void drawAxes();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

signals:

};

#endif // PLOTCANVAS_H
