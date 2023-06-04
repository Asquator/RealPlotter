#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <QString>
#include <QMap>
#include <QSharedPointer>

#include "function_entry.h"

/**
 * @brief The FunctionTable class Convenience class to store named functions
 */
class FunctionTable{

public:
    explicit FunctionTable();
    void replace(char name, QSharedPointer<FunctionEntry> function_ptr);
    QSharedPointer<FunctionEntry> find(char name) const;
	void remove(char name);
    int size(){return functions.size();}

private:
    QMap<char, QSharedPointer<FunctionEntry>> functions;

};



#endif // SYMBOLTABLE_H


