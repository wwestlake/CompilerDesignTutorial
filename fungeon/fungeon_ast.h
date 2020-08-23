#ifndef __FUNGEON_H
#define __FUNGEON_H

#include <ostream>
#include <vector>

class Visitor;

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


class FngNode {
public:

    virtual void visit(Visitor* visitor) = 0;

    virtual std::ostream& out(std::ostream& os) const = 0;    

    friend std::ostream& operator<<(std::ostream& os, const FngNode& node)
    {
        return node.out(os);
    }


};






#endif // !__FUNGEON_H