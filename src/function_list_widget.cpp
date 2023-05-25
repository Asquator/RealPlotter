#include "function_list_widget.h"

FunctionListWidget::FunctionListWidget(QWidget *parent)
	: QWidget(parent),
	  model(new FunctionListModel(this)),
	  delegate(new FunctionEntryDelegate(this)),
      funcListView(new FunctionListView(this)),
      namedFunctions(new FunctionTable(this))
{


	funcListView->setModel(model);

	connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), funcListView,
		SLOT(onRowsInserted(QModelIndex,int,int)));



	//connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), delegate,
	//	SLOT(onRowsInserted(QModelIndex,int,int)));

	//connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), funcListView,
	//	SLOT(onRowsRemoved(QModelIndex,int,int)));

	funcListView->setItemDelegate(delegate);

	delegate->setEditorType(FunctionEntryDelegate::EditorType::DummyLast);
	model->insertRow(0);

	delegate->setEditorType(FunctionEntryDelegate::EditorType::Entry);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(funcListView);

	layout->setContentsMargins(0,0,0,0);
	funcListView->setContentsMargins(0,0,0,0);

	setLayout(layout);
}
