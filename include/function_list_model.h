#ifndef FUNCTIONLISTMODEL_H
#define FUNCTIONLISTMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QSharedPointer>
#include <QThreadPool>

#include "function_entry.h"
#include "function_table.h"

class FunctionListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    FunctionListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data (const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                    int role = Qt::EditRole) override;

signals:
	void lastRemoved();
    void parsedFunction(QModelIndex);


public slots:
	void addEmptyLine();
    void removeLine(const QPersistentModelIndex &);

private:
    QVector<QSharedPointer<FunctionEntry>> funcList;
    FunctionTable namedFunctions;

    QThreadPool parseThreadPool;

    void tryParse(const QModelIndex &index);

	bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    void reparseAll();

private slots:
    void handleParsed(const QModelIndex &index);
};

#endif // FUNCTIONLISTMODEL_H
