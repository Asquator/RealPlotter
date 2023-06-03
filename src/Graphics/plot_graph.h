#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

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

    void addToPlot(const QModelIndex &index);
    void removeFromPlot(const QModelIndex &index);

private:
    QMap<QSharedPointer<FunctionEntry>, QGraphicsPathItem *> functions;

    PlotScene *canvasScene;
    PlotView *canvasView;
};

#endif // PLOTMANAGER_H
