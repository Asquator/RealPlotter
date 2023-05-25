#ifndef FUNCTIONLISTWIDGET_H
#define FUNCTIONLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "function_list_view.h"
#include "line_editor.h"
#include "function_entry_delegate.h"
#include "function_list_model.h"
#include "function_table.h"

class FunctionListWidget : public QWidget
{
	Q_OBJECT
public:
	explicit FunctionListWidget(QWidget *parent = nullptr);

signals:

private:
	FunctionListView *funcListView;
	FunctionListModel *model;
	FunctionEntryDelegate *delegate;
    FunctionTable *namedFunctions;
};

#endif // FUNCTIONLISTWIDGET_H
