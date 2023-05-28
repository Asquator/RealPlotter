#ifndef FUNCTION_ENTRY_H
#define FUNCTION_ENTRY_H

#include <QString>
#include <QMetaType>

#include<RealFunctionLib/real_function.h>

class FunctionEntry{

public:
    FunctionEntry(){qRegisterMetaType<FunctionEntry>();}
    explicit FunctionEntry(const QString &, const RealFunctionLib::RealFunction &);
    explicit FunctionEntry(const QString &, const RealFunctionLib::RealFunction &, char name);

    inline QString getString() const{
        return inputString;
    }

    inline void setString(const QString &str){
        inputString = str;
    }

    inline void setFunction(const RealFunctionLib::RealFunction &func){
        function = func;
    }

    inline bool isValid() const{
        return function.isValid();
    }

    inline bool isNamed() const{
        return name != '\0';
    }

    inline char getName() const{
        return name;
    }

    inline void setName(char newname) {
        name = newname;
    }

    inline void removeName(){
        name = '\0';
    }

    inline RealFunctionLib::RealFunction getFunction() const{
        return function;
    }

private:
    char name = '\0';
    QString inputString;
    RealFunctionLib::RealFunction function;
};


Q_DECLARE_METATYPE(FunctionEntry)


#endif // FUNCTION_ENTRY_H
