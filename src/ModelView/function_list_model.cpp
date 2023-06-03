#include <QSharedPointer>

#include "function_list_model.h"
#include "parse_worker.h"
#include "function_entry.h"

#include <iostream>

using namespace RealFunctionLib;

Q_DECLARE_METATYPE(QSharedPointer<FunctionEntry>)

FunctionListModel::FunctionListModel(QObject *parent):
    QAbstractListModel(parent) {
}


int FunctionListModel::rowCount(const QModelIndex &parent) const {
    return funcList.count();
}

/*
 * Retrieves and returns the shared pointer from the model at the given index
 */
QVariant FunctionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= funcList.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant::fromValue(funcList[index.row()]);

    else
        return QVariant();
}


QVariant FunctionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return QStringLiteral("Functions").arg(section);
}

/*
 * inserts rows to the model at the given position
 */
bool FunctionListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);


    for (int row = 0; row < rows; ++row) {
        funcList.insert(position, QSharedPointer<FunctionEntry>::create());
    }

    endInsertRows();
    return true;
}


Qt::ItemFlags FunctionListModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

/*
 * receives a QVariant with a string, saves the data in the model and tries to parse the new entry
 */
bool FunctionListModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        //if an entry doesn't exist, create a new one
        if(!funcList[index.row()])
            funcList.replace(index.row(), QSharedPointer<FunctionEntry>{new FunctionEntry});

        //set the new string
        funcList[index.row()]->setString(value.toString());

        //try parse the string in a separate thread
        tryParse(index);

        emit dataChanged(index, index, {role});


        return true;
    }

    return false;
}

/*
 * tries to parse the entry at the given index as a function, on success passes control to handleparsed
 */
void FunctionListModel::tryParse(const QModelIndex &index){
    QPersistentModelIndex persIndex = index;
    bool parsed = false;

    //parser task
    ParseWorker *parseWorker = new ParseWorker(funcList[index.row()]->getString(), namedFunctions);

    //on successful parsing, save the result in the model and do further processing
    connect(parseWorker, &ParseWorker::parsed, this,
            [=, &parsed, this](const FunctionEntry &entry){

        //if the index is valid, insert the parsed function entry to the list at the given index
        if(persIndex.isValid()){
            *funcList[persIndex.row()] = entry; //save in the model
            std::cout << "parsed" << std::endl;
            std::cout << funcList[persIndex.row()]->getFunction() << std::endl;

            handleParsed(index);    //further processing if needed

            emit parsedFunction(funcList[persIndex.row()]);
            parsed = true;
        }
    });

    if(!parsed)
        emit invalidated(funcList.at(persIndex.row()));

    parseThreadPool.start(parseWorker);
}

/*
 * processes the newly parsed entry at the given index:
 */
void FunctionListModel::handleParsed(const QModelIndex &index){
    QSharedPointer<FunctionEntry> ptr = funcList[index.row()];

    //if the function is named, then save it to the table of named functions
    std::cout << "checking named" << std::endl;
    if(ptr->isNamed()){
        std::cout << "named: " << ptr->getName() << std::endl;
        namedFunctions.replace(ptr->getName(), ptr);

    }
}


bool FunctionListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    bool reparse = false;

    for (int row = 0; row < rows; ++row) {
        //if removing a named function, remove it also from the reference table and set reparse flag to true

        if(funcList.at(position)->isNamed()){
            reparse = true;
            namedFunctions.remove(funcList.at(position)->getName());
        }

        emit invalidated(funcList.at(position));
        funcList.removeAt(position);
    }

    endRemoveRows();

    //if a named function was removed from the model, reparse everything
    if(reparse)
        reparseAll();

    return true;
}

/*
 * reparses all functions
 */
void FunctionListModel::reparseAll(){
    for(int i = 0; i < funcList.size(); ++i)
        tryParse(index(i));
}

/*
 * adds empty line at the end of the list
 */
void FunctionListModel::addEmptyLine(){
	insertRow(rowCount() - 1);
}

/*
 * removes a line at the specified row from the odel
 */
void FunctionListModel::removeRow(const QPersistentModelIndex &index){
    QAbstractListModel::removeRow(index.row());
}
