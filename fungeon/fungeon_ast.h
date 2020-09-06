#ifndef __FUNGEON_H
#define __FUNGEON_H

#include <ostream>
#include <vector>
#include "visitable.h"

enum class Types {
    INT, 
    FLOAT, 
    STRING, 
    CHAR, 
    BYTE, 
    BOOL, 
    RECORD, 
    ENUM,
    FUNC,
    INFER,
    UNIT
};

enum class Operators {
    PLUS,             
    MINUS,
    TIMES,             
    DIVIDE,           
    MOD,              
    EQ,               
    NOT_EQ,           
    LT,               
    GT,               
    LT_EQ,            
    GT_EQ,
    NOT,
    INCREMENT,
    DECREMENT            
};


static std::string type_to_string(Types type)
{
    switch (type)
    {
        case Types::INT: return "int"; 
        case Types::FLOAT: return "float"; 
        case Types::STRING: return "string"; 
        case Types::CHAR: return "char"; 
        case Types::BYTE: return "byte"; 
        case Types::BOOL: return "bool"; 
        case Types::RECORD: return "record"; 
        case Types::ENUM: return "enum";
        case Types::FUNC: return "func";
        case Types::INFER: return "infer";
        case Types::UNIT: return "unit";

    }
    return "";
}

static std::string operator_to_string(Operators op)
{
    switch (op)
    {
        case Operators::PLUS:           return std::string("+");        
        case Operators::MINUS:          return std::string("-");
        case Operators::TIMES:          return std::string("*");   
        case Operators::DIVIDE:         return std::string("/");  
        case Operators::MOD:            return std::string("%");  
        case Operators::EQ:             return std::string("=");  
        case Operators::NOT_EQ:         return std::string("!=");  
        case Operators::LT:             return std::string("<");  
        case Operators::GT:             return std::string(">");  
        case Operators::LT_EQ:          return std::string("<=");  
        case Operators::GT_EQ:          return std::string(">=");
        case Operators::NOT:            return std::string("!");
        case Operators::INCREMENT:      return std::string("++");
        case Operators::DECREMENT:      return std::string("--");      
    }    
}


class FngNode : public Visitable {
public:
};

typedef std::vector<FngNode*> FngNodeList;


class Expression : public FngNode 
{
};


class RValue : public Expression {
};

typedef std::vector<RValue*> RValueList;

class LValue : public RValue {

};


class FngLiteralBase : public RValue {};

template <typename T> 
class FngLiteral : public FngLiteralBase {
protected:
    Types _type;
    T _value;

public:
    FngLiteral(Types type, T value) : _type(type), _value(value) {}

    Types getType() { return _type; }
    T getValue() { return _value; }

    virtual void accept(Visitor* visitor);

};


class Identifier : public LValue {
protected:
    std::string _ident;
    Types _type;

public:
    Identifier(std::string ident, Types type) : _ident(ident), _type(type) {}

    std::string getIdent() { return _ident; }
    Types getType() { return _type; }
    void setType(Types type) { _type = type; }

    virtual void accept(Visitor* visitor);

};


class Parameter : public Identifier {
public:
    Parameter(std::string ident, Types type) : Identifier(ident, type) {}
    virtual void accept(Visitor* visitor);

};

typedef std::vector<Parameter*> ParameterList;



class FunctionCall : public RValue {
protected:
    std::string _ident;
    RValueList* _args;


public:
    FunctionCall(std::string ident, RValueList* args) : _ident(ident), _args(args) {}
    virtual void accept(Visitor* visitor);
};


class BinaryExpression : public Expression {
protected:
    Expression* _left;
    Expression* _right;
    Operators   _operator;

public:
    BinaryExpression(Expression* left, Operators op, Expression* right)
        : _left(left), _operator(op), _right(right) {}

    Expression* getLeft() { return _left; }
    Expression* getRight() { return _right; }
    Operators getOperator() { return _operator; }
    virtual void accept(Visitor* visitor);
};

class UnaryExpression : public Expression 
{
protected:
    Expression* _expr;
    Operators _operator;

public:
    UnaryExpression(Expression* expr, Operators op) : _expr(expr), _operator(op) {}
};

class PreUnaryExpression : public UnaryExpression {
public:
    PreUnaryExpression(Expression* expr, Operators op) : UnaryExpression(expr, op) {}
    virtual void accept(Visitor* visitor);

};

class PostUnaryExpression : public UnaryExpression {
public:
    PostUnaryExpression(Expression* expr, Operators op) : UnaryExpression(expr, op) {}
    virtual void accept(Visitor* visitor);

};

class TrinaryExpression : public Expression {
protected:
    Expression* _condition;
    Expression* _true_expr;
    Expression* _false_expr;

public:
    TrinaryExpression(Expression* condition, Expression* true_expr, Expression* false_expr) 
        : _condition(condition), _true_expr(true_expr), _false_expr(false_expr) {}

    virtual void accept(Visitor* visitor);

};

class LetStatement : public FngNode {
protected:
    std::string _ident;
    ParameterList* _params;
    FngNodeList* _nodes;
public:
    LetStatement(std::string ident, ParameterList* params, FngNodeList* nodes) : _ident(ident), _params(params), _nodes(nodes) {}

    virtual void accept(Visitor* visitor);

    std::string getIdent() { return _ident; }
    ParameterList* getParams() { return _params; }
    FngNodeList* getNodes() { return _nodes; }

};



#endif // !__FUNGEON_H