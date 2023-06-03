#include "plot_graph.h"
#include "function_list_model.h"
#include <QHBoxLayout>

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

    functions.insert(entryPtr, nullptr);
}

void PlotGraph::removeFromPlot(const QModelIndex &index){
    FunctionListModel *listModel = static_cast<FunctionListModel *>(sender());

    auto entryPtr = listModel->data(index, Qt::DisplayRole).value<QSharedPointer<FunctionEntry>>();

    functions.remove(entryPtr);
}
