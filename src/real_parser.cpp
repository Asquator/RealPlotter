#include <RealFunctionLib/real_function.h>
#include <QString>
#include <deque>
#include <map>
#include <set>
#include <stack>
#include "real_parser.h"

using std::stack;
using std::deque;

using RealFunctionLib::RealFunction;
using RealFunctionLib::Specials;

using TokenType = RealToken::Type;
using OperType = Operator::Type;
using Assoc = Operator::Assoc;

using namespace RealFunctionLib;


//Special functions' string names
std::map<QString, Specials> specials
{
	{"sin", Specials::Sin},
	{"cos", Specials::Cos},
    {"tan", Specials::Tan},
    {"ln", Specials::Ln},
	{"sin", Specials::Sin},
    {"exp", Specials::Exp}
};

//Operator data for special functions
const Operator specialOperator{Assoc::Right, 8, true};

const static std::set<char> operatorsSym {'+','-','/','*','^', '\'', '='};

std::map<OperType, Operator> aritOperators{
    {OperType::Multiply, Operator{Assoc::Left, 5, false, OperType::Multiply}},
    {OperType::Add, Operator{Assoc::Left, 1, false, OperType::Add}},
    {OperType::Subtract, Operator{Assoc::Left, 1, false, OperType::Subtract}},
    {OperType::Minus, Operator{Assoc::Right, 10, true, OperType::Minus}},
    {OperType::Plus, Operator{Assoc::Right, 10, true, OperType::Plus}},
    {OperType::Divide, Operator{Assoc::Left, 5, false, OperType::Divide}},
    {OperType::Power, Operator{Assoc::Right, 15, false, OperType::Power}},
};

//get an operator that represents the given special function
static Operator getSpecialOp(Specials spec){
    return Operator {Assoc::Right, 20, true, OperType::Special, .special=spec};

}

//get an operator that represents a function with an id name
static Operator getIdOp(char id){
    return Operator {Assoc::Right, 20, true, OperType::Id, .id=id};
}


const static std::map<QString, Preserved> preserved{
    {"x", Preserved::x},
    {"y", Preserved::y},
    {"e", Preserved::e},
	{"pi", Preserved::pi},
};

bad_parse_exception::bad_parse_exception(const std::string &s):
	runtime_error(s){}



//RealToken

RealToken::RealToken():
    type{TokenType::Invalid}{}


bool RealToken::isValid() const{
    return type != TokenType::Invalid;
}

bool RealToken::isFunction() const{
    return (type == Type::Operator && oper.type == OperType::Special ||
            oper.type == OperType::Id);
}

bool RealToken::isUnaryOper() const{
    return type == TokenType::Operator && oper.unary;
}

bool RealToken::isIdOper() const{
    return type == TokenType::Operator && oper.type == OperType::Id;
}

bool RealToken::isExprInfix() const{
    return type == Type::Number || type == Type::LeftPar || type == Type::Preserved || isFunction();
}

bool RealToken::isExprPostfix() const{
    return type == Type::Number || type == Type::RightPar || type == Type::Preserved;
}


//RealParser

RealParser::RealParser(const QString &str, const FunctionTable &table):
    inputString(str), table(table), it(str.cbegin()){}


/**
 * @brief RealParser::parse parse the input string
 * @return parsed function entry, invalid if the input is incorrect
 */
FunctionEntry RealParser::parse() {
    deque tokenQue = stringToTokens();
    stripFunctionId(tokenQue);

    ShuntingYardParser postfixConverter{tokenQue};
    tokenQue = postfixConverter.getPostfix();

    PostfixEvaluator evaluatorRPN(tokenQue, table);

    if(named)
        return FunctionEntry(inputString, evaluatorRPN.evaluate(), functionId);

    return FunctionEntry(inputString, evaluatorRPN.evaluate());

}


void RealParser::setString(const QString &inputString){
	this->inputString = inputString;
}

/**
 * @brief RealParser::skipSpaces skips till the first non-space character is encountered
 */
void RealParser::skipSpaces() {
    while(it != inputString.cend() && it->isSpace())
        ++it;
}

/**
 * @brief RealParser::atEnd checks if the iteratoris at the end
 * @return true iff the string exceeded
 */
bool RealParser::atEnd() const{
    return it == inputString.cend();
}

/**
 * @brief RealParser::stripFunctionId strips the given token deque of a function name declaration, a.k.a "f(x) ="
 * the name is saved in the functionId field
 * @param q deque of tokens to strip
 */
void RealParser::stripFunctionId(deque<RealToken> &q){
    if(q.size() < 2)
        return;

    //get rid of "y = "
    if(q[0].type == TokenType::Preserved && q[0].pres == Preserved::y
        && q[1].type == TokenType::Equals){
        q.erase(q.begin(), q.begin() + 2);
        return;
    }

    if(q.size() < 5 || !q[0].isIdOper())
        return;

    char id = q[0].oper.id;

    //check and strip of " f(x) = " pattern
    if(q[1].type == TokenType::LeftPar &&
        q[2].type == TokenType::Preserved && q[2].pres == Preserved::x &&
        q[3].type == TokenType::RightPar &&
        q[4].type == TokenType::Equals)
    {
        q.erase(q.begin(), q.begin() + 5);
        functionId = id;
        named = true;
    }
}

RealToken RealParser::readNumber(){
    RealToken token;

	bool found = false;

    QString str;

    while(!atEnd() && it->isDigit())
        str += *it++;

    if(*it == '.'){
        str += *it++;
        while(!atEnd() && it->isDigit())
            str += *it++;
    }

	bool ok;
	double val = str.toDouble(&ok);

	if(ok){
		token.type = TokenType::Number;
		token.num = val;
        nextOpUnary = false;
	}

    return token;
}


RealToken RealParser::readSpecial(){
    RealToken token;
    bool found = false;

    auto itSaved = it;

    for(QString str; !found && !atEnd() && it->isLetter(); ++it){
        str += *it;
        auto itSpecial = specials.find(str);
        if (itSpecial != specials.end()) { // if found
			token.type = TokenType::Operator;
            token.oper.type = Operator::Type::Special;
			token.oper = getSpecialOp(itSpecial->second);
            nextOpUnary = true;
            found = true;
        }
    }

    if(!found)
        it = itSaved;

    return token;
}

RealToken RealParser::readArithmeticOperator(){
	RealToken token;
	if(operatorsSym.find(it->toLatin1()) != operatorsSym.end()){
		token.type = TokenType::Operator;
		switch(it->toLatin1()){
			case '+':
                token.oper = nextOpUnary ? aritOperators[OperType::Plus] : aritOperators[OperType::Add];
				break;

			case '-':
                token.oper = nextOpUnary ? aritOperators[OperType::Minus] : aritOperators[OperType::Subtract];
				break;

			case '*':
                token.oper = aritOperators[OperType::Multiply];
				break;

			case '/':
                token.oper = aritOperators[OperType::Divide];
				break;

			case '^':
                token.oper = aritOperators[OperType::Power];
				break;

			case '=':
				token.type = TokenType::Equals;
                break;
        }
    ++it;
    nextOpUnary = true;
	}

	return token;
}


RealToken RealParser::readPreserved(){
    RealToken token;
    bool found = false;

    auto itSaved = it;

    for(QString str; !found && !atEnd() && it->isLetter(); ++it){
        str += *it;
        auto itPreserved = preserved.find(str);
        if (itPreserved != preserved.end()) { // if found
            token.type = TokenType::Preserved;
            token.pres = itPreserved->second;
            found = true;
        }
    }

    if(!found)
        it = itSaved;

    nextOpUnary = false;
    return token;
}

RealToken RealParser::readPar(){
    RealToken token;
    if(*it == '('){
        token.type = TokenType::LeftPar;
        nextOpUnary = true;
    }

    else if (*it == ')'){
        token.type = TokenType::RightPar;
        nextOpUnary = false;
    }

    if(token.isValid())
        ++it;

    return token;
}

RealToken RealParser::readId(){
	RealToken token;

    if(it->isLetter()){
        token.type = TokenType::Operator;
        token.oper = getIdOp(it->toLatin1());
		++it;

        nextOpUnary = true;
	}

	return token;
}

int RealParser::readCountDiffTags(){
    int cnt = 0;
    while(!atEnd() && *it == '\''){
        ++it;
        ++cnt;
    }

    return cnt;
}

/*
bool RealParser::readStripXArgument(){
    auto itSaved = it;
    if(it + 2 < inputString.cend() &&
        *it++ == '(' && *it++ == 'x' && *it++ == ')')
        return true;

    it = itSaved;
    return false;
}
*/

RealToken RealParser::nextToken(){
    RealToken token;

    skipSpaces();

    //returning an invalid token if no more characters to read
    if(it == inputString.cend()) return token;
    else if((token = readArithmeticOperator()).isValid()) return token;
    else if((token = readPar()).isValid()) return token;
    else if((token = readNumber()).isValid()) return token;
    else if((token = readSpecial()).isValid()) return token;
    else if((token = readPreserved()).isValid()) return token;
    else if((token = readId()).isValid()){

        //if function, count derivative operators and save the diff order
        if(token.isFunction())
            token.oper.derivativeOrder = readCountDiffTags();

    }
    return token;

}


bool Operator::pushedFirst(const Operator &other){
	if (precedence > other.precedence)
		return true;

    return (precedence == other.precedence && other.assoc == Assoc::Left);
}


deque<RealToken> RealParser::stringToTokens(){
	deque<RealToken> tokenQue;

    //multiply token, inserted explicitly where implied
    RealToken multiply;
    multiply.type = TokenType::Operator;
    multiply.oper = aritOperators[OperType::Multiply];

    //whether the last token can serve as a postfix of a full expression, used to insert multiply tokens
    bool lastPostfix = false;

    for(RealToken tok = nextToken(); tok.isValid(); tok = nextToken()){

        //insert multiply between full sequential expressions
        if(lastPostfix && tok.isExprInfix())

        tokenQue.push_back(multiply);

        lastPostfix = tok.isExprPostfix();

        tokenQue.push_back(tok);
    }

	return tokenQue;
}


//Shunting yard parser

RealParser::ShuntingYardParser::ShuntingYardParser(const deque<RealToken> &input):
    input(input){}

void RealParser::ShuntingYardParser::treatRightPar(const RealToken &token){
    while(!opStack.empty() && opStack.top().type != TokenType::LeftPar){
        output.push_back(opStack.top());
        opStack.pop();
    }

    //discard the left par if it's present
    if(!opStack.empty())
        opStack.pop();

    //reduceUnaries();
}

/*
void RealParser::ShuntingYardParser::reduceUnaries(){
    while(!opStack.empty() && opStack.top().isUnaryOper()){
        output.push_back(opStack.top());
        opStack.pop();
    }

}

*/

void RealParser::ShuntingYardParser::treatOperator(const RealToken &token){
    //push to the output tokens with greater precedence, and put the given token to opstack
    while(!opStack.empty() &&
           opStack.top().type == TokenType::Operator &&
           opStack.top().oper.pushedFirst(token.oper)){

        output.push_back(opStack.top());
        opStack.pop();
    }

    opStack.push(token);
}

deque<RealToken> RealParser::ShuntingYardParser::getPostfix(){
    RealToken token;
    Operator op;

    for(const RealToken &token : input){
        switch(token.type){
        case TokenType::Number:
        case TokenType::Preserved:
            output.push_back(token);
            break;

        case TokenType::LeftPar:
            opStack.push(token);
            break;

        case TokenType::RightPar:
            treatRightPar(token);
            break;

        case TokenType::Operator:
            treatOperator(token);
            break;


        default:
            throw bad_parse_exception("invalid token");
        }
    }

    while(!opStack.empty()){
        if(opStack.top().type != TokenType::LeftPar)
            output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}

RealParser::PostfixEvaluator::PostfixEvaluator(const deque<RealToken> &rpn, const FunctionTable &table) :
    rpn(rpn), table(table){}

RealFunction RealParser::PostfixEvaluator::evaluate(){
    for(const auto &token : rpn){
        switch(token.type){

        case TokenType::Number:
            operands.push(createConstant(token.num));
            break;

        case TokenType::Preserved:
            treatPreserved(token);
            break;

        case TokenType::Operator:
            treatOperator(token);
            break;
        }
    }

    return operands.size() == 1 ? operands.top() : RealFunction{};

}

void RealParser::PostfixEvaluator::treatPreserved(const RealToken &token){
    switch(token.pres){
    case Preserved::x:
        operands.push(getSpecial(Specials::Id));
        break;

    case Preserved::e:
        operands.push(createConstant(Real_Math::E));
        break;

    case Preserved::pi:
        operands.push(createConstant(Real_Math::PI));

    default:
        throw bad_parse_exception("invalid preserved token in expression");
    }
}


void RealParser::PostfixEvaluator::treatOperator(const RealToken &token){
    if(!token.oper.unary){ //binary operators
        if(operands.size() < 2)
            throw bad_parse_exception("too few operands");

        RealFunction op2 = operands.top();
        operands.pop();

        RealFunction op1 = operands.top();
        operands.pop();

        switch(token.oper.type){
        case OperType::Add:
            operands.push(op1 + op2);
            break;

        case OperType::Subtract:
            operands.push(op1 - op2);
            break;

        case OperType::Multiply:
            operands.push(op1 * op2);
            break;

        case OperType::Divide:
            operands.push(op1 / op2);
            break;

        case OperType::Power:
            operands.push(op1.power(op2));
            break;
        }

        return;
    }

    if(operands.empty())
        throw bad_parse_exception("too few operands");

    switch(token.oper.type){

    case OperType::Minus:
            operands.top() = -operands.top();
        break;

    case OperType::Special:
    {
        RealFunction kernel = getSpecial(token.oper.special).getDerivative(token.oper.derivativeOrder); //get the special function and differentiate it as much as needed
        operands.top() = kernel(operands.top()); //combine the kernel with the previous operand
    }
        break;

    case OperType::Id:
        if(QSharedPointer<FunctionEntry> ptr = table.find(token.oper.id)){
            RealFunction kernel = ptr->getFunction().getDerivative(token.oper.derivativeOrder);
            operands.top() = kernel(operands.top());
        }

        else throw bad_parse_exception("undefined identificator " + token.ch);
        break;
    }

}
