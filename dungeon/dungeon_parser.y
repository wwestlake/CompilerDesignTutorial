%{

#define YY_BUF_SIZE 32768

#include <stdio.h>    
#include <iostream>
#include <string>
#include <vector>
//#include "dungeon.h"
#include "dungeon_parser.hpp"
using namespace std;

extern int dnglex();
extern int dngparse();


extern FILE* dngin;

int yyerror(const char * p)
{
    cerr << p << endl;
}



%}
%locations

%define api.prefix {dng}

%define parse.trace
%define parse.error verbose

%union {
    int intval;
    double floatval;
    char* str;
};

%token DUNGEON ROOMS ROOM NAME DESC EXITS

%token<floatval> FLOAT
%token<intval>   INT
%token<str> IDENT

%token T_PLUS T_MINUS T_DIVIDE T_MULTIPLY 
%token T_ROOM T_DOOR 
%token<str> T_STRING
%token<str> T_TEXT

%type<floatval> float_expr
%type<intval> int_expr

%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%%

dungeon:
    DUNGEON IDENT ';' 
        rooms  
        room_defs
    ;

room_defs:
    room
    | room_defs room
    ;

rooms:
    ROOMS ident_list ';'
    ;

room:
    ROOM IDENT ':'
        NAME T_STRING ';'
        DESC T_STRING ';'
        EXITS ident_list ';'
    ;

 ident_list:
    '[' list_of_idents ']'   
    ;

list_of_idents:
    IDENT
    | list_of_idents ',' IDENT
    ;

float_expr:                           
    FLOAT                                   { $$ = $1; }
    | float_expr T_PLUS float_expr          { $$ = $1 + $3; }
    | float_expr T_MINUS float_expr         { $$ = $1 - $3; }
    | float_expr T_MULTIPLY float_expr      { $$ = $1 * $3; }
    | float_expr T_DIVIDE float_expr        { $$ = $1 / $3; }
    | '(' float_expr ')'                    { $$ = $2; }
    | T_PLUS float_expr                     { $$ = $2; }
    | T_MINUS float_expr                    { $$ = -$2; }
    | int_expr                              { $$ = (double)$1; }
    ;

int_expr:                           
    INT                                   { $$ = $1; }
    | int_expr T_PLUS int_expr              { $$ = $1 + $3; }
    | int_expr T_MINUS int_expr             { $$ = $1 - $3; }
    | int_expr T_MULTIPLY int_expr          { $$ = $1 * $3; }
    | int_expr T_DIVIDE int_expr            { $$ = $1 / $3; }
    | '(' int_expr ')'                     { $$ = $2; }
    | T_PLUS int_expr                       { $$ = $2; }
    | T_MINUS int_expr                      { $$ = -$2; }
    ;



%%

int dng_eval(std::string filename)
{
    FILE * input =  fopen(filename.c_str(), "r");
    if (! input)
    {
        std::cout << "Can't open file: " << filename << std::endl;
        return -1;
    }       

    dngin = input;
    return dngparse();

}


