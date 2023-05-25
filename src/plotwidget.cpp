#include <QVBoxLayout>
#include <QHBoxLayout>

#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent)
    :QWidget(parent),
    model(new FunctionListModel(this)),
    delegate(new FunctionEntryDelegate(this)),
    funcListView(new FunctionListView(this)),
    canvas(new PlotCanvas(this))
{

    funcListView->setModel(model);

    //ensuring opening a persistent line editor in the view
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), funcListView,
            SLOT(onRowsInserted(QModelIndex,int,int)));

    funcListView->setItemDelegate(delegate);

    delegate->setEditorType(FunctionEntryDelegate::EditorType::DummyLast);
    model->insertRow(0);

    delegate->setEditorType(FunctionEntryDelegate::EditorType::Entry);

	//layout management
    QHBoxLayout *layout = new QHBoxLayout;
	QSplitter *splitter = new QSplitter;

    splitter->addWidget(funcListView);
	splitter->addWidget(canvas);

	layout->addWidget(splitter);

	canvas->setContentsMargins(0,0,0,0);
    funcListView->setContentsMargins(0,0,0,0);
	splitter->setContentsMargins(0,0,0,0);
	layout->setContentsMargins(0,0,0,0);

	setLayout(layout);

}

PlotWidget::~PlotWidget() {}


