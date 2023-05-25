#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPaintEvent>
#include <QRect>

class PlotCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit PlotCanvas(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *);

private:
    QRect region;

signals:

};

#endif // PLOTCANVAS_H
