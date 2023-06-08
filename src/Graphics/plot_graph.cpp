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

    canvasScene->requestNewCenter(QPointF{100,100});
    canvasView->setScene(canvasScene);
    canvasView->centerOn(canvasScene->sceneRect().center());


    connect(canvasView, SIGNAL(viewChanged()), this, SLOT(refreshAll()));
    connect(canvasView, SIGNAL(zoomed(double)), canvasScene, SLOT(updateGridUnits(double)));

    connect(canvasView->horizontalScrollBar(), &QScrollBar::valueChanged, this, &PlotGraph::horizontalMoved);
    connect(canvasView->verticalScrollBar(), &QScrollBar::valueChanged, this, &PlotGraph::verticalMoved);

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


void PlotGraph::horizontalMoved(int newVal)
{
    QScrollBar *bar = static_cast<QScrollBar *>(sender());
    /*
    double oldX;

    if(newVal == bar->minimum()){
        QRectF vis = canvasView->visibleRect();

        double x_shift = canvasScene->sceneRect().width();

        canvasScene->requestExpandLeft(x_shift);
        //canvasView->translate(x_shift, 0);
    }
*/
}


void PlotGraph::verticalMoved(int newVal)
{
    QScrollBar *bar = static_cast<QScrollBar *>(sender());
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
