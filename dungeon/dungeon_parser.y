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

%token DUNGEON ROOM NAME DESC EXITS UNIQUE ITEM VALUE TEXT
%token ITEMS

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
    DUNGEON IDENT '{' 
        dungeon_items
    '}'
    ;

dungeon_items:
    dungeon_item
    | dungeon_items dungeon_item
    ;

dungeon_item:
    description
    | item
    | unique_item
    | room
    ;

item:
    ITEM IDENT '{' description VALUE '=' INT '}'
    ;

unique_item:
    UNIQUE ITEM IDENT '{' description VALUE '=' INT '}'
    ;

room:
    ROOM IDENT '{' 
        room_items
        '}'
    ;

room_items:
    room_item
    | room_items room_item
    ;

room_item:
    description
    | exit_list
    | item_list
    ;

exit_list:
    EXITS ident_list
    ;    

item_list:
    ITEMS ident_list
    ;

description:
    DESC '{' description_items '}'
    ;

description_items:
    /* empty */
    | display_name 
    | display_name text
    ;

display_name:
    NAME T_STRING
    ;

text:
    TEXT T_STRING
    ;

ident_list:
    '[' ident_list_items ']'
    ;

ident_list_items:
    IDENT
    | ident_list_items ',' IDENT
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


