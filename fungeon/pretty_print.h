#ifndef __PRETTY_PRINT_H
#define __PRETTY_PRINT_H

#include "visitor.h"

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
    virtual void Visit(Identifier* node) override               
    { 

    }

    virtual void Visit(Parameter* node) override                
    { 
        std::cout << "(" << node->getIdent() << " : " << type_to_string( node->getType() ) << ") ";
    }

    virtual void Visit(FunctionCall* node) override             
    { 

    }
    
    virtual void Visit(LetStatement* node) override             
    { 
        indent();
        std::cout << "Let " << node->getIdent() << " "; 
        if (node->getParams() != nullptr) for (auto param : *node->getParams())
        {
            param->accept(this);
        }
        std::cout << "{" << std::endl;
        push_indent();
        if (node->getNodes() != nullptr) for (auto stmt : *node->getNodes())
        {
            stmt->accept(this);
        }
        std::cout << std::endl;
        pop_indent();
        indent();
        std::cout << "}" << std::endl;

    }
    virtual void Visit(BinaryExpression* node) override         { }
    virtual void Visit(PreUnaryExpression* node) override       { }
    virtual void Visit(PostUnaryExpression* node) override      { }
    virtual void Visit(FngLiteralBase* node) override           { }
    virtual void Visit(TrinaryExpression* node) override        { }

    virtual void Visit(FngLiteral<bool>* node) {}
    virtual void Visit(FngLiteral<char>* node) {}
    virtual void Visit(FngLiteral<double>* node) {}
    virtual void Visit(FngLiteral<int>* node) {}
    virtual void Visit(FngLiteral<std::string>* node) {}

};


#endif // !__PRETTY_PRINT_H

