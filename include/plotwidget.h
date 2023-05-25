#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSplitter>

#include "function_list_view.h"
#include "line_editor.h"
#include "function_entry_delegate.h"
#include "function_list_model.h"
#include "function_table.h"
#include "plot_canvas.h"


class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    PlotWidget(QWidget *parent = nullptr);
    ~PlotWidget();

private:
    FunctionListView *funcListView;
    FunctionListModel *model;
    FunctionEntryDelegate *delegate;
    PlotCanvas *canvas;

    FunctionTable namedFunctions;
};
#endif // PLOTWIDGET_H
