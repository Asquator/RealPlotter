#include "function_list_view.h"
#include "function_list_model.h"

FunctionListView::FunctionListView(QWidget *parent) :
	QListView(parent){
    setMinimumWidth(100);
    setMaximumWidth(INT_MAX);

    //disable item selection in a view
    setSelectionMode(QAbstractItemView::NoSelection);
}

void FunctionListView::onRowsInserted(const QModelIndex &parent, int first, int last){
	FunctionListModel *listModel = static_cast<FunctionListModel *>(model());
	for(int i = first; i <= last; i++)
		openPersistentEditor(listModel->index(i));
}



