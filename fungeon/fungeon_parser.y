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
    
    std::cerr << "fungeon: Error! - In file: " << filename << ": On line: " << fungeon_lloc.last_line << " (" << p << ")" <<  std::endl; 
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
    Types type;
    Identifier* typed_identifier;
    ParameterList* param_list;
    Parameter* param;

};

%token ERROR LET COLON QUEST IF THEN ELSE OPAREN CPAREN OBRACE CBRACE SEMI_COLON COMMA
%token INCREMENT DECREMENT PLUS TIMES DIVIDE MOD EQ ASSIGN NOT_EQ NOT LAMBDA MINUS INTO BIND KLEISLY_BIND
%token COMPOSE BEFORE AFTER GT_EQ LT_EQ GT  LT TYPE

%token INT_T FLOAT_T STRING_T CHAR_T BYTE_T BOOL_T RECORD_T ENUM_T FUNC_T  

%token<op_val> OP SCOPE_OP
%token<symbol> IDENT
%token<string_val>  STRING_VAL  
%token<inum_val>    INT_VAL     
%token<fnum_val>    FLOAT_VAL   
%token<char_val>    CHAR_VAL    
%token<bool_val>    BOOL_VAL    

%type<type> type unit
%type<typed_identifier> identifier typed_identifier
%type<param> param;
%type<param_list> param_list


%left PLUS MINUS
%left TIMES DIVIDE MOD



%%

program:
    /* empty */
    | program line
    | error
    ;

line:
    let_statement
    | fun_call
    | expr
    ;

let_statement:
    LET IDENT ASSIGN body
    | LET IDENT param_list ASSIGN body
    ;

body:
    line
    | OBRACE stmt_list CBRACE
    ;

stmt_list:
    line
    | stmt_list line
    ;

 lvalue:
    identifier
    ;

rvalue:
    lvalue
    | literal
    ;

literal:
    STRING_VAL
    | INT_VAL
    | FLOAT_VAL
    | CHAR_VAL
    | BOOL_VAL
    ;

rvalue_list:
    rvalue
    | rvalue_list rvalue
    ;

fun_call:                   
    IDENT unit                     
    | IDENT rvalue_list             
    | OPAREN fun_call CPAREN        
    ;

expr:
    rvalue                       
    | expr PLUS expr             
    | expr MINUS expr            
    | expr TIMES expr            
    | expr DIVIDE expr           
    | expr MOD expr              
    | expr EQ expr               
    | expr NOT_EQ expr           
    | expr LT expr               
    | expr GT expr               
    | expr LT_EQ expr            
    | expr GT_EQ expr            
    | MINUS expr                 
    | PLUS expr                  
    | NOT expr                   
    | IF expr THEN expr ELSE expr
    | INCREMENT lvalue           
    | DECREMENT lvalue           
    | lvalue INCREMENT           
    | lvalue DECREMENT           
    | fun_call
    | OPAREN expr CPAREN         
    ;

param:
    identifier                  { $$ = (Parameter*)$1; }
    | typed_identifier          { $$ = (Parameter*)$1; }
    ;

param_list:
    param                   { $$ = new ParameterList(); $$->push_back($1); DEBUG(*$1);}
    | param_list param      { $$ = $1; $$->push_back($2); DEBUG(*$2); }
    ;

typed_identifier:
    OPAREN identifier COLON type CPAREN   { $2->setType($4); $$ = $2; DEBUG(*$$); }  
    ;

identifier:
    IDENT               { $$ = new Identifier(std::string($1), Types::INFER); DEBUG(*$$); }
    ;

type:
    INT_T               { $$ = Types::UNIT;     }
    | FLOAT_T           { $$ = Types::FLOAT;    }
    | STRING_T          { $$ = Types::STRING;   }    
    | CHAR_T            { $$ = Types::CHAR;     }
    | BYTE_T            { $$ = Types::BYTE;     }    
    | BOOL_T            { $$ = Types::BOOL;     }
    | RECORD_T          { $$ = Types::RECORD;   }
    | ENUM_T            { $$ = Types::ENUM;     }
    ;

 unit:
    | OPAREN CPAREN { $$ = Types::UNIT; }
    ;






%%



