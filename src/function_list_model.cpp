#include "function_list_model.h"
#include "parse_worker.h"
#include "iostream"

using namespace RealFunctionLib;

FunctionListModel::FunctionListModel(QObject *parent):
    QAbstractListModel(parent) {
}


int FunctionListModel::rowCount(const QModelIndex &parent) const {
    return funcList.count();
}


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

/**
 * @brief FunctionListModel::tryParse tries to parse the newly inserted line in a separate thread
 * @param index index of the inserted line
 */
void FunctionListModel::tryParse(const QModelIndex &index){
    QPersistentModelIndex persIndex = index;

    //parser task
    ParseWorker *parseWorker = new ParseWorker(funcList[index.row()]->getString(), namedFunctions);

    //on successful parsing, save the result in the model and do further processing
    connect(parseWorker, &ParseWorker::parsed, this,
            [=, this](const FunctionEntry &entry){

        //if the index is valid, insert the parsed function entry to the list at the given index
        if(persIndex.isValid()){
            *funcList[persIndex.row()] = entry; //save in the model
            std::cout << "parsed" << std::endl;
            std::cout << funcList[persIndex.row()]->getFunction() << std::endl;

            handleParsed(index);    //further processing if needed
        }

    });

    parseThreadPool.start(parseWorker);
}

/**
 * @brief FunctionListModel::handleParsed processes the parsed function entry
 * @param index index in the model
 */
void FunctionListModel::handleParsed(const QModelIndex &index){
    QSharedPointer<FunctionEntry> ptr = funcList[index.row()];

    std::cout << "checking named" << std::endl;
    if(ptr->isNamed()){
        std::cout << "named: " << ptr->getName() << std::endl;
        namedFunctions.replace(ptr->getName(), ptr);

    }
}

/**
 * @brief FunctionListModel::removeRows removes rows from the list model
 * @param position from
 * @param rows number of rows to remove
 * @param parent parent, not used in the sequential model
 * @return true if successfully removed
 */
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

        funcList.removeAt(position);
    }

    endRemoveRows();

    if(reparse)
        reparseAll();

    return true;
}

void FunctionListModel::reparseAll(){
    for(int i = 0; i < funcList.size(); ++i)
        tryParse(index(i));
}

/**
 * @brief FunctionListModel::addEmptyLine adds empty line at the end of the list
 */
void FunctionListModel::addEmptyLine(){
	insertRow(rowCount() - 1);
}

/**
 * @brief FunctionListModel::removeLine removes line at the given index
 * @param index index in the model
 */
void FunctionListModel::removeLine(const QPersistentModelIndex &index){
    removeRow(index.row());
}
