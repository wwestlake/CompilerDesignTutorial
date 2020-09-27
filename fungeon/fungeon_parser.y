%{

#include <stdio.h>    
#include <iostream>
#include <string>
#include <map>
#include <cstdlib> //-- I need this for atoi
#include <cstdint>
#include <list>
#include <vector>
#include <map>
#include "fungeon_ast.h"
#include "fungeon_parser.hpp"

extern FILE *funcle_in;
bool errors = false;
std::string filename;

#define DEBUG(A) std::cout << A; std::cout.flush(); do {} while (false)

int fungeon_error(const char *p) 
{ 
    
    std::cerr << "fungeon: Error! - In file: " << filename << ": in column: " << fungeon_lloc.last_column <<  " On line: " << fungeon_lloc.last_line << " (" << p << ")" <<  std::endl; 
    errors = true;
    return 0;
}

int fungeon_warning(const char *p) 
{ 
    std::cerr << "fungeon: Warning - " << p << " " <<  std::endl; 
    errors = true;
    return 0;
}

//-- Lexer prototype required by bison, aka getNextToken()
int fungeon_lex(); 


FngNodeList* fng_nodes = new FngNodeList();

%}

%locations
%define api.prefix {fungeon_}
%define parse.trace
%define parse.error verbose


%union {
    std::string* identifier;
    int inum_val;
    double fnum_val;
    bool bool_val;
    char char_val;
    char* symbol;
    std::string* string_val;
    std::string* error_message;
    char* op_val;
    Type* type;
    Identifier* typed_identifier;
    ParameterList* param_list;
    Parameter* param;
    Expression* expression;
    LValue* lvalue;
    RValue* rvalue;
    FunctionCall* fun_call;
    RValueList* rvalue_list;
    LetStatement* let_statement;
    PrintStatement* print_statement;
    FngNode* node;
    FngNodeList* node_list;
    Tuple* tuple;
    List* list;
    RecordField* record_field;
    RecordFieldList* record_field_list;
    Record* record;
    EnumField* enum_field;
    EnumFieldList* enum_field_list;
    Enum* enumeration;
};

%token ERROR LET COLON QUEST IF THEN ELSE OPAREN CPAREN OBRACE CBRACE SEMI_COLON COMMA OBRACKET CBRACKET
%token INCREMENT DECREMENT PLUS TIMES DIVIDE MOD EQ ASSIGN NOT_EQ NOT LAMBDA MINUS INTO BIND KLEISLY_BIND
%token COMPOSE BEFORE AFTER GT_EQ LT_EQ GT  LT TYPE UNIT WITH OF PIPE RIGHT_ARROW RECORD ENUMERATION

%token INT_T FLOAT_T STRING_T CHAR_T BYTE_T BOOL_T FUNC_T PRINT_T 

%token<op_val> OP SCOPE_OP
%token<symbol> IDENT
%token<symbol> TYPE_IDENT
%token<string_val>  STRING_VAL  
%token<inum_val>    INT_VAL     
%token<fnum_val>    FLOAT_VAL   
%token<char_val>    CHAR_VAL    
%token<bool_val>    BOOL_VAL    

%type<type> type unit
%type<typed_identifier> identifier typed_identifier
%type<param> param;
%type<param_list> param_list
%type<expression> expr
%type<lvalue> lvalue 
%type<rvalue> rvalue literal
%type<fun_call> fun_call
%type<rvalue_list> rvalue_list tuple_pair tuple_rvalue_list
%type<let_statement> let_statement
%type<print_statement> print_statement
%type<node> line
%type<node_list> program body stmt_list
%type<tuple> tuple
%type<list> list

%type<record_field> record_field
%type<record_field_list> record_field_list
%type<record> record

%type<enum_field> enum_field
%type<enum_field_list> enum_field_list
%type<enumeration> enum


%left PLUS MINUS
%left TIMES DIVIDE MOD



%%

program:
    /* empty */                         {  }
    | program line                      { fng_nodes->push_back($2); }
    | error                             {  }
    ;

line:
    let_statement                       { $$ = $1; }
    | fun_call                          { $$ = $1; }
    | print_statement                   { $$ = $1; }
    | expr                              { $$ = $1; }
    | line SEMI_COLON                   { $$ = $1; }
    | enum                              { $$ = $1; }
    | record                            { $$ = $1; }
    ;

print_statement:
    PRINT_T expr                        { $$ = new PrintStatement($2); }
    ;

let_statement:
    LET IDENT ASSIGN body               { $$ = new LetStatement($2, nullptr, $4); }
    | LET IDENT param_list ASSIGN body  { $$ = new LetStatement($2, $3, $5); }
    ;

body:
    line                        { $$ = new FngNodeList(); $$->push_back($1); }
    | OBRACE stmt_list CBRACE   { $$ = $2; }
    ;

stmt_list:
    line                { $$ = new FngNodeList(); $$->push_back($1); }
    | stmt_list line    { $$ = $1; $$->push_back($2); }
    ;


literal:
    STRING_VAL      { $$ = new FngLiteral<std::string>( new Type( Types::STRING ), *$1); }
    | INT_VAL       { $$ = new FngLiteral<int>(new Type( Types::INT ), $1); }
    | FLOAT_VAL     { $$ = new FngLiteral<double>(new Type( Types::FLOAT ), $1); }
    | CHAR_VAL      { $$ = new FngLiteral<char>(new Type(Types::CHAR), $1); }
    | BOOL_VAL      { $$ = new FngLiteral<bool>(new Type(Types::BOOL), $1); }
    ;


fun_call:                   
    identifier OPAREN rvalue_list  CPAREN      { $$ = new FunctionCall($1, $3); }
    ;

expr:
    literal                       { $$ = $1; }
    | lvalue                        { $$ = $1; }
    | fun_call                      { $$ = $1; }
    | expr PLUS expr                { $$ = new BinaryExpression($1, Operators::PLUS, $3); }     
    | expr MINUS expr               { $$ = new BinaryExpression($1, Operators::MINUS, $3); }
    | expr TIMES expr               { $$ = new BinaryExpression($1, Operators::TIMES, $3); }
    | expr DIVIDE expr              { $$ = new BinaryExpression($1, Operators::DIVIDE, $3); }
    | expr MOD expr                 { $$ = new BinaryExpression($1, Operators::MOD, $3); }
    | expr EQ expr                  { $$ = new BinaryExpression($1, Operators::EQ, $3); }
    | expr NOT_EQ expr              { $$ = new BinaryExpression($1, Operators::NOT_EQ, $3); }
    | expr LT expr                  { $$ = new BinaryExpression($1, Operators::LT, $3); }
    | expr GT expr                  { $$ = new BinaryExpression($1, Operators::GT, $3); }
    | expr LT_EQ expr               { $$ = new BinaryExpression($1, Operators::LT_EQ, $3); }
    | expr GT_EQ expr               { $$ = new BinaryExpression($1, Operators::GT_EQ, $3); }
    | MINUS expr                    { $$ = new PreUnaryExpression($2, Operators::MINUS); }
    | PLUS expr                     { $$ = new PreUnaryExpression($2, Operators::PLUS); }
    | NOT expr                      { $$ = new PreUnaryExpression($2, Operators::NOT); }
    | IF expr THEN expr ELSE expr   { $$ = new TrinaryExpression($2, $4, $6); }
    | INCREMENT lvalue              { $$ = new PreUnaryExpression($2, Operators::INCREMENT); }
    | DECREMENT lvalue              { $$ = new PreUnaryExpression($2, Operators::DECREMENT); }
    | lvalue INCREMENT              { $$ = new PostUnaryExpression($1, Operators::INCREMENT); }
    | lvalue DECREMENT              { $$ = new PostUnaryExpression($1, Operators::DECREMENT); }
    | OPAREN expr CPAREN            { $$ = $2; }
    | tuple                         { $$ = $1; }
    | list                          { $$ = $1; }
      
    ;

lvalue:
    identifier      { $$ = $1; }
    ;


rvalue_list:
    rvalue                         { $$ = new RValueList(); $$->push_back($1); }
    | rvalue_list COMMA rvalue     { $$ = $1; $$->push_back($3); }
    ;


rvalue:
    expr                            { $$ = new RValue($1); }
    | OPAREN rvalue CPAREN          { $$ = $2; }
    ;

tuple_pair:
    rvalue COMMA rvalue             { $$ = new RValueList(); $$->push_back($1); $$->push_back($3); }    
    ;

tuple_rvalue_list:
    tuple_pair                      { $$ = $1; }
    | tuple_rvalue_list COMMA rvalue { $$ = $1; $$->push_back($3); }
    ;

tuple:
    OPAREN tuple_rvalue_list CPAREN { $$ = new Tuple($2); }
    ;

list:
    OBRACKET CBRACKET                   { $$ = new List(); }
    | OBRACKET rvalue_list CBRACKET     { $$ = new List($2); }
    ;


param:
    identifier                  { $$ = new Parameter($1, new Type(Types::INFER) ); }
    | typed_identifier          { $$ = new Parameter($1, $1->getType()); }
    ;

param_list:
    param                   { $$ = new ParameterList(); $$->push_back($1);}
    | param_list param      { $$ = $1; $$->push_back($2);  }
    | unit                  { $$ = new ParameterList(); }
    ;

typed_identifier:
    OPAREN identifier COLON type CPAREN   { $2->setType($4); $$ = $2;  }  
    ;

identifier:
    IDENT               { $$ = new Identifier(std::string($1), new Type( Types::INFER ));  }
    ;



type:
    INT_T               { $$ = new Type( Types::INT );     }
    | FLOAT_T           { $$ = new Type( Types::FLOAT );    }
    | STRING_T          { $$ = new Type( Types::STRING );   }    
    | CHAR_T            { $$ = new Type( Types::CHAR );     }
    | BYTE_T            { $$ = new Type( Types::BYTE );     }    
    | BOOL_T            { $$ = new Type( Types::BOOL );     }
    ;

record_field:
    identifier COLON type SEMI_COLON    { $$ = new RecordField($1, $3); }
    ;

record_field_list:
    record_field                        { $$ = new RecordFieldList(); $$->push_back($1); }
    | record_field_list record_field    { $$ = $1; $$->push_back($2); }
    ;

record:
    RECORD TYPE_IDENT ASSIGN OBRACE record_field_list CBRACE
        {
            DEBUG("Found record");
            $$ = new Record(new Identifier( $2, new Type( Types::RECORD )), $5);
        }
    | error
    ;

enum_field:
    PIPE TYPE_IDENT SEMI_COLON              { $$ = new EnumField(new Identifier( $2, new Type( Types::UNIT ) ), new Type( Types::UNIT )); }
    | PIPE TYPE_IDENT OF type SEMI_COLON    { $$ = new EnumField(new Identifier( $2, $4 ), $4); }
    ;

enum_field_list:
    enum_field                              { $$ = new EnumFieldList(); $$->push_back($1); }
    | enum_field_list enum_field            { $$ = $1; $$->push_back($2); }
    ;

enum:
    ENUMERATION TYPE_IDENT ASSIGN OBRACE enum_field_list CBRACE
        {
            DEBUG("Found Enumeration");
            $$ = new Enum(new Identifier( $2, new Type( Types::ENUM )), $5);
        }
    | error
    ;



 unit:
    UNIT { $$ = new Type( Types::UNIT ); }
    ;



%%






