#include "fungeon_ast.h"
#include "visitor.h"

template<>
void FngLiteral<bool>::accept(Visitor* visitor) { visitor->Visit(this); }

template<>
void FngLiteral<char>::accept(Visitor* visitor) { visitor->Visit(this); }

template<>
void FngLiteral<double>::accept(Visitor* visitor) { visitor->Visit(this); }

template<>
void FngLiteral<int>::accept(Visitor* visitor) { visitor->Visit(this); }

template<>
void FngLiteral<std::string>::accept(Visitor* visitor) { visitor->Visit(this); }


void Identifier::accept(Visitor* visitor) { visitor->Visit(this); }
void Parameter::accept(Visitor* visitor) { visitor->Visit(this); }
void FunctionCall::accept(Visitor* visitor) { visitor->Visit(this); }
void BinaryExpression::accept(Visitor* visitor) { visitor->Visit(this); }
void PreUnaryExpression::accept(Visitor* visitor) { visitor->Visit(this); }
void PostUnaryExpression::accept(Visitor* visitor) { visitor->Visit(this); }
void TrinaryExpression::accept(Visitor* visitor) { visitor->Visit(this); }
void LetStatement::accept(Visitor* visitor) { visitor->Visit(this); }

void RValue::accept(Visitor* visitor) { visitor->Visit(this); }
void PrintStatement::accept(Visitor* visitor) { visitor->Visit(this); }
void Tuple::accept(Visitor* visitor)  { visitor->Visit(this); }
void List::accept(Visitor* visitor)  { visitor->Visit(this); }
void Record::accept(Visitor* visitor)  { visitor->Visit(this); }
void Enum::accept(Visitor* visitor)  { visitor->Visit(this); }

