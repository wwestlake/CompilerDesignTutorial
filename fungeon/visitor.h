#ifndef __VISITOR_H
#define __VISITOR_H

#include "fungeon_ast.h"

class Visitor {
public:

    virtual void Visit(Identifier* node) = 0;
    virtual void Visit(Parameter* node) = 0 ;
    virtual void Visit(FunctionCall* node) = 0;
    virtual void Visit(LetStatement* node) = 0;
    virtual void Visit(PrintStatement* node) = 0;
    virtual void Visit(BinaryExpression* node) = 0;
    virtual void Visit(PreUnaryExpression* node) = 0;
    virtual void Visit(PostUnaryExpression* node) = 0;
    virtual void Visit(TrinaryExpression* node) = 0;
    virtual void Visit(RValue* node) = 0;


    virtual void Visit(FngLiteral<bool>* node) = 0;
    virtual void Visit(FngLiteral<char>* node) = 0;
    virtual void Visit(FngLiteral<double>* node) = 0;
    virtual void Visit(FngLiteral<int>* node) = 0;
    virtual void Visit(FngLiteral<std::string>* node) = 0;



    void Visit(Visitable* root)
    {
        root->accept(this);
    }

};


#endif // __VISITOR_H