#include "function_table.h"

FunctionTable::FunctionTable(){}


void FunctionTable::replace(char name, QSharedPointer<FunctionEntry> function_ptr){
	functions.insert(name, function_ptr);
}

void FunctionTable::remove(char name){
	functions.remove(name);
}

QSharedPointer<FunctionEntry> FunctionTable::find(char name) const{
	auto it = functions.find(name);
    return it != functions.end() ? *it : nullptr;
}
