#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPaintEvent>
#include <QRect>
#include <QGraphicsScene>


class PlotScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PlotScene(QWidget *parent = nullptr);



private:
    QRect region;

signals:

};

#endif // PLOTCANVAS_H
