#include <RealFunctionLib/real_function.h>

#include "parse_worker.h"
#include "real_parser.h"

using RealFunctionLib::RealFunction;

ParseWorker::ParseWorker(const QString &str, const FunctionTable &table) :
    input(str), namedFunctions(table){}

void ParseWorker::run() try {
    RealParser parser(input, namedFunctions);
    FunctionEntry res = parser.parse();

    if(res.isValid())
        emit parsed(res);

} catch (const bad_parse_exception &ex){}
