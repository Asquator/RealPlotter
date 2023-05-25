#ifndef FUNCTION_LIST_VIEW_H
#define FUNCTION_LIST_VIEW_H

#include <QListView>

class FunctionListView : public QListView {

    Q_OBJECT

public:
    FunctionListView(QWidget *parent = nullptr);

    //QSize sizeHint() const;

public slots:
	void onRowsInserted(const QModelIndex &parent, int first, int last);


private:


};

#endif // FUNCTION_LIST_VIEW_H
