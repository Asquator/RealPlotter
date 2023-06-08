#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

#include <vector>

#include <QWidget>
#include <QGraphicsPathItem>
#include <QMap>
#include <QSharedPointer>
#include <QGraphicsPathItem>

#include <RealFunctionLib/real_function.h>

#include "function_entry.h"
#include "plot_scene.h"
#include "plot_view.h"

class PlotGraph : public QWidget
{

    Q_OBJECT

public:
    PlotGraph(QWidget *parent);

public slots:
    void addRefreshPlot(QSharedPointer<FunctionEntry>);
    void removeFromPlot(QSharedPointer<FunctionEntry>);

    void horizontalMoved(int newVal);
    void verticalMoved(int newVal);

    void refreshAll();

private:
    static constexpr double DELTA_RATIO = 10000;

    QMap<QSharedPointer<FunctionEntry>, QGraphicsPathItem *> functions;

    PlotScene *canvasScene;
    PlotView *canvasView;

    QPainterPath buildPath(const std::vector<real_type> &values, real_type x_start, real_type real_delta);
};

#endif // PLOTMANAGER_H
