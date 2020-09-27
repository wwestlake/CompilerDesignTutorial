#ifndef __PRETTY_PRINT_H
#define __PRETTY_PRINT_H

#include "visitor.h"

//#define PRT(A) std::cout << A << std::endl
#define PRT(A) do {} while (false)

std::string reescape(char c)
{
    switch (c)
    {
        case '\n' : return "\\n";
        case '\t' : return "\\t";
        case '\v' : return "\\v";
    }

    std::string result;

    result[0] = c;
    return result;
}


class PrettyPrint : public Visitor {
    int _indent = 0;
    int _indent_inc = 4;

    void indent()
    {
        for (int i=0; i < _indent; i++)
            std::cout << " ";
    }

    void push_indent() { _indent += _indent_inc; }
    void pop_indent() { _indent -= _indent_inc; }

public:

    virtual void Visit(Type* node) override               
    { 
        std::cout << type_to_string( node->getType() );
    }

    virtual void Visit(TupleType* node) override               
    { 
        std::cout << "(tuple ";
        int len = node->getTypes()->size();
        int i = 0;
        for (auto n : *node->getTypes())
        {
            n->accept(this);
            i++;
            if (i < len) std::cout << " * ";
        }
        std::cout << ")";
    }

    virtual void Visit(ListType* node) override               
    { 
        std::cout << "(";
        node->getListType()->accept(this);
        std::cout << " list)";
    }

    virtual void Visit(Identifier* node) override               
    { 
        PRT("identifier");
        std::cout << node->getIdent();
    }

    virtual void Visit(CustomType* node) override               
    { 
        node->getIdent()->accept(this);
    }


    virtual void Visit(Parameter* node) override                
    { 
        PRT("parameter");
        std::cout << "(" << node->getIdent()->getIdent() << " : " << type_to_string( node->getType()->getType() ) << ")";
    }

    virtual void Visit(RValue* node) override 
    {
        PRT("rvalue");
        if (node->getExpr() != nullptr)
            node->getExpr()->accept(this);
    }

    virtual void Visit(Tuple* node) override 
    {
        std::cout << "(";
        int len = node->getRValues()->size();
        int i = 0;
        for (auto n : *node->getRValues())
        {
            i++;
            n->accept(this);
            if (i < len) 
            {
                std::cout << ", ";
            }
        }
        std::cout << ")";
    }

    virtual void Visit(List* node) override 
    {
        std::cout << "[";
        int len = node->getRValues()->size();
        int i = 0;
        for (auto n : *node->getRValues())
        {
            i++;
            n->accept(this);
            if (i < len) 
            {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }


    virtual void Visit(FunctionCall* node) override             
    { 
        PRT("function call");
        std::cout << "(" << node->getIdent()->getIdent() << " ";
        if (node->getArgs() != nullptr)
        {
            for (auto arg : *node->getArgs())
            {
                arg->accept(this);
                std::cout << " ";
            }
        } else {
            std::cout << "()";
        }
        std::cout << ")";
    }
    
    virtual void Visit(LetStatement* node) override             
    { 
        PRT("let statement");
        std::cout << "Let " << node->getIdent() << " "; 
        if (node->getParams() != nullptr) for (auto param : *node->getParams())
        {
            param->accept(this);
            std::cout << " ";
        }
        std::cout << " = {" << std::endl;
        push_indent();
        if (node->getNodes() != nullptr) for (auto stmt : *node->getNodes())
        {
            indent();
            stmt->accept(this);
            std::cout << std::endl;
        }
        std::cout << std::endl;
        pop_indent();
        indent();
        std::cout << "}" << std::endl;

    }

    virtual void Visit(PrintStatement* node) override 
    {
        std::cout << "print ";
        node->getExpr()->accept(this);
    }

    virtual void Visit(BinaryExpression* node) override         
    { 
        PRT("binary expression");
        node->getLeft()->accept(this);
        std::cout << " " << operator_to_string( node->getOperator() ) << " ";
        node->getRight()->accept(this);
    }
    virtual void Visit(PreUnaryExpression* node) override       
    { 
        PRT("pre unary expression");
        std::cout << operator_to_string( node->getOperator() );
        node->getExpr()->accept(this);
    }

    virtual void Visit(PostUnaryExpression* node) override      
    { 
        PRT("post unary expression");
        node->getExpr()->accept(this);
        std::cout << operator_to_string( node->getOperator() );
    }
    
    virtual void Visit(TrinaryExpression* node) override        
    { 
        PRT("trinary expression");
        std::cout << "if ";
        node->geCondition()->accept(this);
        std::cout << " then ";
        node->getTrueExpr()->accept(this);
        std::cout << " else " ;
        node->getFalseExpr()->accept(this);

    }

    virtual void Visit(FngLiteral<bool>* node)  override
    {
        PRT("literal bool");
        std::cout << node->getValue();
    }



    virtual void Visit(FngLiteral<char>* node)  override
    {
        PRT("literal char");
        std::cout << "'" << reescape( node->getValue() ) << "'";
    }
    virtual void Visit(FngLiteral<double>* node)  override
    {
        PRT("literal double");
        std::cout << node->getValue();
    }
    virtual void Visit(FngLiteral<int>* node)  override
    {
        PRT("literal int");
        std::cout << node->getValue();
    }
    virtual void Visit(FngLiteral<std::string>* node)  override
    {
        PRT("literal string");
       std::cout << node->getValue();
    }

    virtual void Visit(Record* node)  override
    {
        std::cout << "type " << node->getIdent()->getIdent() << " = {" << std::endl;
        push_indent();
        for (auto n : *node->getFieldList())
        {
            indent();
            std::cout << n->getIdent()->getIdent() << " : ";
            n->getType()->accept(this);
            std::cout << ";" << std::endl;
        }
        pop_indent();
        std::cout << "}";
    }

    virtual void Visit(Enum* node)  override
    {
        std::cout << "type " << node->getIdent()->getIdent() << " = {" << std::endl;
        push_indent();
        for (auto n : *node->getFieldList())
        {
            indent();
            std::cout << "|" << n->getIdent()->getIdent() << " of ";
            n->getType()->accept(this);
            std::cout << ";" << std::endl;
        }
        pop_indent();
        std::cout << "}";
    }



};


#endif // !__PRETTY_PRINT_H

