#ifndef FUNCTIONLISTMODEL_H
#define FUNCTIONLISTMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QSharedPointer>
#include <QThreadPool>
#include <QSharedPointer>

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
    void parsedFunction(QSharedPointer<FunctionEntry>);
    void invalidated(QSharedPointer<FunctionEntry>);


public slots:
    /**
     * @brief addEmptyLine adds empty line at the end of the list
     */
	void addEmptyLine();

    /**
     * @brief removeRow removes line at the given index
     * @param index index in the model
     */
    void removeRow(const QPersistentModelIndex &);

private:
    QVector<QSharedPointer<FunctionEntry>> funcList;
    FunctionTable namedFunctions;

    QThreadPool parseThreadPool;

    void tryParse(const QModelIndex &index);

	bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    void reparseAll();

private slots:

    /**
     * @brief handleParsed processes the parsed function entry
     * @param index index in the model
     */
    void handleParsed(const QModelIndex &index);
};

#endif // FUNCTIONLISTMODEL_H
