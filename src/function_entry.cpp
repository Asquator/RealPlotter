#include "function_entry.h"
#include "real_parser.h"

using RealFunctionLib::RealFunction;

FunctionEntry::FunctionEntry(const QString &str, const RealFunction &function):
    inputString(str), function(function), name{'\0'} {}

FunctionEntry::FunctionEntry(const QString &str, const RealFunction &function, char name):
    inputString(str), function(function), name(name){}



