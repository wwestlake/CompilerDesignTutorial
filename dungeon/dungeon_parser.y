%{

#define YY_BUF_SIZE 32768

#include <stdio.h>    
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <tuple>

//#include "dungeon.h"
#include "../include/dungeon.h"
#include "dungeon_parser.hpp"


extern int dnglex();
extern int dngparse();


extern FILE* dngin;

int yyerror(const char * p)
{
    std::cerr << p << std::endl;
}

static Dungeon* dungeon_result;


%}
%locations

%define api.prefix {dng}

%define parse.trace
%define parse.error verbose

%union {
    int intval;
    double floatval;
    char* str;
    std::vector<std::string>* ident_list;
    std::string* text;
    Dungeon* dungeon;
    Room* room;
    Item* item;
    DungeonNode* node;
    std::vector<DungeonNode*>* node_list;
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
%type<ident_list> ident_list_items
%type<ident_list> ident_list
%type<text> text
%type<text> display_name
%type<ident_list> item_list
%type<ident_list> exit_list
%type<room> room room_item room_items
%type<item> unique_item item
%type<dungeon> dungeon
%type<node> description dungeon_item
%type<node_list> dungeon_items

%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%%

dungeon:
    DUNGEON IDENT '{' 
        dungeon_items
    '}'

    {
        auto ident = $2;
        DungeonNode* desc = nullptr;

        for (auto node : *$4)
        {
            if (node->getNodeType() == NodeType::Description)
            {
                desc = node;
                break;
            }
        }
        if (desc != nullptr) 
        {
            $$ = new Dungeon(ident, desc->getDisplayName(), desc->getDescription());
            for (auto node : *$4)
            {
                switch (node->getNodeType())
                {
                    case NodeType::Room: $$->addRoom(static_cast<Room*>( node )); break;
                    case NodeType::Item: $$->addItem(static_cast<Item*>( node )); break;
                }
            }


        } else {
            yyerror("No description for dungoen given");
            return -1;
        }
        dungeon_result = $$;
    }

    ;

dungeon_items:
    dungeon_item                    { $$ = new std::vector<DungeonNode*>(); $$->push_back($1); }
    | dungeon_items dungeon_item    { $$->push_back($2); }
    ;

dungeon_item:
    description     { $$ = $1; }
    | item          { $$ = $1; }
    | unique_item   { $$ = $1; }
    | room          { $$ = $1; }
    ;

item:
    ITEM IDENT '{' description VALUE '=' INT '}' 
    {
        $$ = new Item($2, $4->getDisplayName(), $4->getDescription(), $7, ItemType::Common);
    }
    ;

unique_item:
    UNIQUE ITEM IDENT '{' description VALUE '=' INT '}' 
    {
        $$ = new Item($3, $5->getDisplayName(), $5->getDescription(), $8, ItemType::Unique);
    }
    ;

room:
    ROOM IDENT 
        '{' 
            description                 
            exit_list                 
            item_list 
        '}'             
        {
            $$ = new Room($2, $4);
            $$->addExits(*$5);
            $$->addItems(*$6);
        }
    ;



exit_list:
    EXITS ident_list              { $$ = $2; }      
    ;    

item_list:
    ITEMS ident_list               { $$ = $2;  }     
    ;

description:
    DESC '{' display_name text '}' { $$ = new DungeonNode("", *$3, *$4, NodeType::Description); }     
    ;


display_name:
    NAME T_STRING                  { $$ = new std::string($2); }     
    ;

text:
    TEXT T_STRING                  { $$ = new std::string($2); }                    
    ;

ident_list:
    '[' ident_list_items ']'       { $$ = $2; }       
    ;

ident_list_items:
    IDENT                          { $$ = new std::vector<std::string>(); $$->push_back($1); }     
    | ident_list_items ',' IDENT   { $$ = $1; $$->push_back($3); }     
    ;


%%

std::tuple<int, Dungeon*> dng_eval(std::string filename)
{
    dungeon_result = nullptr;
    FILE * input =  fopen(filename.c_str(), "r");
    if (! input)
    {
        std::cout << "Can't open file: " << filename << std::endl;
        return std::tuple<int, Dungeon*>(-1, nullptr);
    }       

    dngin = input;
    return std::tuple<int, Dungeon*>(dngparse(), dungeon_result);

}


