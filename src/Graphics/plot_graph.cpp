#include <future>

#include <QList>
#include <QHBoxLayout>
#include <QScrollBar>

#include "plot_graph.h"
#include "function_list_model.h"

using namespace RealFunctionLib;
using std::future;

PlotGraph::PlotGraph(QWidget *parent) : QWidget(parent), canvasScene(new PlotScene),
    canvasView(new PlotView(this)) {
    canvasView->setScene(canvasScene);

    connect(canvasView, SIGNAL(viewChanged()), this, SLOT(refreshAll()));

    //Layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    canvasView->setContentsMargins(0,0,0,0);
    layout->addWidget(canvasView);

    setLayout(layout);
}



void PlotGraph::addRefreshPlot(QSharedPointer<FunctionEntry> entryPtr){
    RealFunction func = entryPtr->getFunction();

    QRectF rect = canvasView->visibleRect();

    double x1 = canvasScene->mapXToRealCoords(rect.left());
    double x2 = canvasScene->mapXToRealCoords(rect.right());
    double delta = (x2 - x1) / DELTA_RATIO;

    future<std::vector<real_type>> valuesFuture = std::async(std::launch::async, evaluate, func, x1, x2, delta);

    canvasScene->removeItem(functions[entryPtr]);

    removeFromPlot(entryPtr);

    QPen pen;
    pen.setCosmetic(true);

    QPainterPath path = buildPath(valuesFuture.get(), x1, delta);

    QGraphicsPathItem *item = canvasScene->addPath(path, pen);

    functions.insert(entryPtr, item);
}


void PlotGraph::removeFromPlot(QSharedPointer<FunctionEntry> entryPtr){
    canvasScene->removeItem(functions[entryPtr]);

    delete functions[entryPtr];
    functions.remove(entryPtr);
}


void PlotGraph::refreshAll(){
    QList<QSharedPointer<FunctionEntry>> keys = functions.keys();

    for(const auto &ptr : keys)
        addRefreshPlot(ptr);
}


QPainterPath PlotGraph::buildPath(const std::vector<real_type> &values, real_type x, real_type delta)
{
    QPainterPath path;

    if(values.size() == 0)
        return path;

    path.moveTo(canvasScene->mapXToSceneCoords(x), canvasScene->mapYToSceneCoords(values[0]));

    x += delta;

    for(int i = 1; i < values.size(); ++i, x += delta){
        path.lineTo(canvasScene->mapXToSceneCoords(x), canvasScene->mapYToSceneCoords(values[i]));
    }

    return path;
}
