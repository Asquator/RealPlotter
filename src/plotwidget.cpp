#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>

#include "plotwidget.h"
#include "plot_graph.h"

PlotWidget::PlotWidget(QWidget *parent)
    :QWidget(parent),
    model(new FunctionListModel(this)),
    delegate(new FunctionEntryDelegate(this)),
    funcListView(new FunctionListView(this))
{
    setMinimumSize(700,400);

    funcListView->setModel(model);

    //ensuring opening a persistent line editor in the view
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), funcListView,
            SLOT(onRowsInserted(QModelIndex,int,int)));

    funcListView->setItemDelegate(delegate);

    delegate->setEditorType(FunctionEntryDelegate::EditorType::DummyLast);
    model->insertRow(0);

    delegate->setEditorType(FunctionEntryDelegate::EditorType::Entry);

    PlotGraph *plotManager = new PlotGraph(this);

    connect(model, SIGNAL(parsedFunction(QModelIndex)), plotManager,
            SLOT(addToPlot(QModelIndex)));

    connect(model, SIGNAL(invalidated(QModelIndex)), plotManager,
            SLOT(removeFromPlot(QModelIndex)));

	//layout management
    QHBoxLayout *layout = new QHBoxLayout;
	QSplitter *splitter = new QSplitter;

    splitter->addWidget(funcListView);
    splitter->addWidget(plotManager);
    splitter->setSizes(QList<int>{200,600});

	layout->addWidget(splitter);

    plotManager->setContentsMargins(0,0,0,0);
    funcListView->setContentsMargins(0,0,0,0);
	splitter->setContentsMargins(0,0,0,0);
	layout->setContentsMargins(0,0,0,0);

	setLayout(layout);

}

PlotWidget::~PlotWidget() {}


