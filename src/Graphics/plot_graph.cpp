#include <QHBoxLayout>
#include <future>

#include "plot_graph.h"
#include "function_list_model.h"

using namespace RealFunctionLib;
using std::future;

using Axis = PlotScene::Axis;

PlotGraph::PlotGraph(QWidget *parent) : QWidget(parent), canvasScene(new PlotScene),
    canvasView(new PlotView(this)) {

    canvasView->setScene(canvasScene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    canvasView->setContentsMargins(0,0,0,0);
    layout->addWidget(canvasView);

    setLayout(layout);
}

void PlotGraph::addToPlot(const QModelIndex &index){
    FunctionListModel *listModel = static_cast<FunctionListModel *>(sender());

    auto entryPtr = listModel->data(index, Qt::DisplayRole).value<QSharedPointer<FunctionEntry>>();
    RealFunction func = entryPtr->getFunction();

    QRectF rect = canvasView->visibleRect();

    double x1 = canvasScene->mapToRealCoords(rect.left(), Axis::X);
    auto r = rect.right();
    double x2 = canvasScene->mapToRealCoords(rect.right(), Axis::X);
    double delta = (x2 - x1) / DELTA_RATIO;

    future<std::vector<real_type>> valuesFuture = std::async(std::launch::async, evaluate, func, x1, x2, delta);

    auto vals = valuesFuture.get();

    QPainterPath path = buildPath(vals, x1, delta);
    canvasScene->addPath(path);

    functions.insert(entryPtr, nullptr);
}


void PlotGraph::removeFromPlot(const QModelIndex &index){
    FunctionListModel *listModel = static_cast<FunctionListModel *>(sender());

    auto entryPtr = listModel->data(index, Qt::DisplayRole).value<QSharedPointer<FunctionEntry>>();

    functions.remove(entryPtr);
}


QPainterPath PlotGraph::buildPath(const std::vector<real_type> &values, real_type x, real_type delta)
{
    QPainterPath path;

    if(values.size() == 0)
        return path;

    path.moveTo(canvasScene->mapToSceneCoords(x, Axis::X), canvasScene->mapToSceneCoords(values[0], Axis::Y));

    x += delta;

    for(int i = 1; i < values.size(); ++i, x += delta){
        path.lineTo(canvasScene->mapToSceneCoords(x, Axis::X), canvasScene->mapToSceneCoords(values[i], Axis::Y));
    }

    return path;
}
