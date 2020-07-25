#ifndef __AST_H
#define __AST_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Context {
public:
    bool running;
};


class Node {
public:
    virtual std::string toString() = 0;
    virtual void Execute(Context& context) = 0;
};


class Result {
protected:
    std::vector<Node*> _nodes;
    int _result;

public:

    Result(int result) : _result(result) {}

    void AddNode(Node* node) {
        _nodes.push_back(node);
    }

    void AddNodes(std::vector<Node*> nodes)
    {
        for (auto node : nodes)
        {
            AddNode(node);
        }
    }

    std::vector<Node*> getNodes()
    {
        return _nodes;
    }

    int getResult() { return _result; }


};



template <typename T>
class Literal : public Node {
protected: 
    T _value;

public:
    Literal(T value) : _value(value) {}
    T getValue() { return _value; }

    virtual std::string toString()
    {
        std::stringstream ss;
        ss << _value;
        return ss.str();
    }
    virtual void Execute(Context& context) 
    {
        std::cout << this->toString() << std::endl;
    }

};


class Command : public Node 
{
public:
    
};

class CmdQuit : public Command 
{
public:
    virtual void Execute(Context& context)
    {
        context.running = false;
    }

    virtual std::string toString()
    {
        return "";
    }

};





#endif // __AST_H