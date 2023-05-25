#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSplitter>
#include <QGraphicsView>

#include "function_list_view.h"
#include "line_editor.h"
#include "function_entry_delegate.h"
#include "function_list_model.h"
#include "function_table.h"
#include "plot_scene.h"


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

    PlotScene *canvasScene;
    QGraphicsView *canvasView;

    FunctionTable namedFunctions;
};
#endif // PLOTWIDGET_H
