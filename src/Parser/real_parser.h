#ifndef REAL_PARSER_H
#define REAL_PARSER_H

#include <QString>
#include <QTextStream>
#include <QSharedPointer>
#include <deque>
#include <stack>
#include <QPair>

#include <exception>

#include <RealFunctionLib/real_function.h>

#include "function_table.h"
#include "function_entry.h"


enum class Preserved {
	x, y, e, pi
};

//struct for any arithmetic/functional operator
struct Operator{
    enum class Type {
		Plus, //unary plus
		Minus, //unary minus
		Add,
        Subtract,
        Multiply,
        Divide,
        Power,

        //functions, always unary
        Special,
        Id,
    };

	enum class Assoc {
		Left,
		Right
	};

	Assoc assoc;
	int precedence = -1;
    bool unary = false; //true if unary,

    Type type;

    int derivativeOrder; //releveant for function operators, number of times to differentiate before applying

    union{
        char id;
        RealFunctionLib::Specials special;
    };

	bool pushedFirst(const Operator &other);
};


//struct for any real parser token
struct RealToken {
	enum class Type {
        Invalid,
        Operator,
        Equals,
        LeftPar,
        RightPar,
        Preserved, //Preserved identifier
        Number,	//floating point number
    };

    Type type = Type::Invalid;

	union{
        char ch;
		Operator oper;
        Preserved pres;
        real_type num;
	};

    std::string text;

    bool isFunction() const;
    bool isUnaryOper() const;
    bool isIdOper() const;

    //true iff the token may serve as infix/suffix of a full expression
    bool isExprInfix() const;
    bool isExprPostfix() const;

    inline bool isValid() const;

    RealToken();
};


/**
 * @brief The RealParser class provides functionality to parse an expression and get the corresponding real function object
 */
class RealParser {

public:
    explicit RealParser(const QString &str, const FunctionTable &table);

	void setString(const QString &inputString);
    FunctionEntry parse();

private:
    using itqs = QString::const_iterator;

    const FunctionTable &table;

    //string iterator
    itqs it;

    //true iff the following condition holds true: if next token is an operator, then it's unary
	bool nextOpUnary = true;

    //true iff the following condition holds true: if next token is a beginning of expression (a.k.a number, 'x', '('), then multiply is implied
    bool insertMultiply = false;

    bool named = false;
    char functionId;
    QString inputString;


    //auxiliary class to convert infix expressions to postfix ones
    class ShuntingYardParser {
    private:
        std::stack<RealToken> opStack; //operator stack
        std::deque<RealToken> output; //output queue
        const std::deque<RealToken> &input;

        void treatOperator(const RealToken &token);
        void treatRightPar(const RealToken &token);



        //move unary operators from operator stack to output
        //void reduceUnaries();


    public:
        ShuntingYardParser(const std::deque<RealToken> &);

        /**
     * @brief getPostfix convers the given expression to RPN
     * @return queue with postfix representation of the given expression
     */
        std::deque<RealToken> getPostfix();

    };


    //auxiliary class to evaluate postfix expressions
    class PostfixEvaluator{
    private:
        std::stack<RealFunctionLib::RealFunction> operands;
        const std::deque<RealToken> &rpn;
        const FunctionTable &table;

        //auxiliary methods to treat different kinds of tokens
        void treatPreserved(const RealToken &token);
        void treatOperator(const RealToken &token);

    public:
        PostfixEvaluator(const std::deque<RealToken> &rpn, const FunctionTable &table);

        //creates the corresponding real function from postfix expression
        RealFunctionLib::RealFunction evaluate();
    };

    //converts the input string to token queue
    std::deque<RealToken> stringToTokens();

    //return a valid token if it corresponds to the specific type
    RealToken readSpecial();
    RealToken readNumber();
    RealToken readPreserved();
	RealToken readArithmeticOperator();
	RealToken readSavedFunction();
    RealToken readPar();
	RealToken readId();
    //bool readStripXArgument();

    //reads and counts differentiation tag symbols "'"
    int readCountDiffTags();

    inline bool atEnd() const;
	RealToken nextToken();

    void stripFunctionId(std::deque<RealToken> &tokenQue);

    void skipSpaces();
};


//exception to designate a failed parsing
class bad_parse_exception : public std::runtime_error {
public:
	explicit bad_parse_exception(const std::string &s);

};



#endif // REAL_PARSER_H
