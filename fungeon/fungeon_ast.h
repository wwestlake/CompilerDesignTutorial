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
    LIST,
    TUPLE,
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
        case Types::LIST: return "list";
        case Types::TUPLE: return "tuple";
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
    return "";    
}


class FngNode : public Visitable {
public:
};

//class Error : public FngNode {
//    int _col;
//    int _line;
//    std::string _message;
//protected:
//    Error(int col, int line, std::string message) : _col(col), _line(line), _message(message) {}
//    virtual void accept(Visitor* visitor);
//
//};

typedef std::vector<FngNode*> FngNodeList;


class Type : public FngNode {
protected:
    Types _type;
public:
    Type(Types type) : _type(type) {}

    Types getType() { return _type; }

    virtual void accept(Visitor* visitor);


};

typedef std::vector<Type*> TypeList;

class TupleType : public Type {
protected:
    TypeList* _types;
public:
    TupleType(TypeList* types) : Type(Types::TUPLE), _types(types) {}

    TypeList* getTypes() { return _types; } 

    virtual void accept(Visitor* visitor);

};

class ListType : public Type {
public:
    ListType() : Type(Types::LIST) {}

    virtual void accept(Visitor* visitor);

};


class Expression : public FngNode 
{
};


class RValue : public Expression {
private:
    Expression* _expr = nullptr;
public:
    RValue() {};
    RValue(Expression* expr) : _expr(expr) {}

    Expression* getExpr() { return _expr; }

    virtual void accept(Visitor* visitor);

};

typedef std::vector<RValue*> RValueList;

class LValue : public RValue {
};


class FngLiteralBase : public RValue {};

template <typename T> 
class FngLiteral : public FngLiteralBase {
protected:
    Type* _type;
    T _value;

public:
    FngLiteral() : _type(new Type(Types::UNIT)) {}
    FngLiteral(Type* type, T value) : _type(type), _value(value) {}


    Type* getType() { return _type; }
    T getValue() { return _value; }

    virtual void accept(Visitor* visitor);

};

class Tuple : public RValue {
protected:
    RValueList* _rvalues;
public:
    Tuple(RValueList* rvalues) : _rvalues(rvalues) {}

    RValueList* getRValues() { return _rvalues; }
    virtual void accept(Visitor* visitor);

};

class List : public RValue {
protected:
    RValueList* _rvalues;
public:
    List() : _rvalues(new RValueList()) { }
    List(RValueList* rvalues) : _rvalues(rvalues) {}

    RValueList* getRValues() { return _rvalues; }
    virtual void accept(Visitor* visitor);

};




class Identifier : public LValue {
protected:
    std::string _ident;
    Type* _type;

public:
    Identifier(std::string ident, Type* type) : _ident(ident), _type(type) {}

    std::string getIdent() { return _ident; }
    Type* getType() { return _type; }
    void setType(Type* type) { _type = type; }

    virtual void accept(Visitor* visitor);

};


class Parameter : public Identifier {
private:
    Identifier* _ident;

public:
    Parameter(Identifier* ident, Type* type) : Identifier(ident->getIdent(), type), _ident(ident) {}
    virtual void accept(Visitor* visitor);

    Identifier* getIdent() {
        return _ident;
    }

};

typedef std::vector<Parameter*> ParameterList;



class FunctionCall : public RValue {
protected:
    Identifier* _ident;
    RValueList* _args = nullptr;


public:
    FunctionCall(Identifier* ident, RValueList* args) : _ident(ident), _args(args) {}
    virtual void accept(Visitor* visitor);
    Identifier* getIdent() { return _ident; }
    RValueList* getArgs() { return _args; }
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
    Expression* getExpr() { return _expr; }
    Operators getOperator() { return _operator; }

};

class PostUnaryExpression : public UnaryExpression {
public:
    PostUnaryExpression(Expression* expr, Operators op) : UnaryExpression(expr, op) {}
    virtual void accept(Visitor* visitor);
    Expression* getExpr() { return _expr; }
    Operators getOperator() { return _operator; }

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
    Expression* geCondition() { return _condition; }
    Expression* getTrueExpr() { return _true_expr; }
    Expression* getFalseExpr() { return _false_expr; }

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

class PrintStatement : public FngNode {
protected:
    Expression* _expr;
public:
    PrintStatement(Expression* expr) : _expr(expr) {}
    virtual void accept(Visitor* visitor);
    Expression* getExpr() { return _expr; }
};

class RecordField {
protected:
    Identifier* _ident;
    Type* _type;

public:
    RecordField(Identifier* ident, Type* type) : _ident(ident), _type(type) {}

    Identifier* getIdent() { return _ident; }
    Type* getType() { return _type; }
};

typedef std::vector<RecordField*> RecordFieldList;


class Record : public FngNode {
protected:
    Identifier* _ident;
    RecordFieldList* _field_list;
public:
    Record(Identifier* ident, RecordFieldList* field_list) : _ident(ident), _field_list(field_list) {}

    Identifier* getIdent() { return _ident; }
    RecordFieldList* getFieldList() { return _field_list; }

    virtual void accept(Visitor* visitor);


};

class EnumField {
protected:
    Identifier* _ident;
    Type* _type;

public:
    EnumField(Identifier* ident, Type* type) : _ident(ident), _type(type) {}

    Identifier* getIdent() { return _ident; }
    Type* getType() { return _type; }
};

typedef std::vector<EnumField*> EnumFieldList;


class Enum : public FngNode {
protected:
    Identifier* _ident;
    EnumFieldList* _field_list;
public:
    Enum(Identifier* ident, EnumFieldList* field_list) : _ident(ident), _field_list(field_list) {}

    Identifier* getIdent() { return _ident; }
    EnumFieldList* getFieldList() { return _field_list; }

    virtual void accept(Visitor* visitor);


};



#endif // !__FUNGEON_H