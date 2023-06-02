#ifndef PARSER_WORKER_H
#define PARSER_WORKER_H

#include <QObject>
#include <QRunnable>

#include<RealFunctionLib/real_function.h>
#include "function_table.h"
#include "function_entry.h"


class ParseWorker : public QObject, public QRunnable{

    Q_OBJECT

public:
    ParseWorker(const QString &, const FunctionTable &);

    void run() override;

private:
    QString input;
    const FunctionTable &namedFunctions;

signals:
    void parsed(FunctionEntry);

};

#endif // PARSER_WORKER_H
