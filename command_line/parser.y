%{
#include <iostream>
#include <string>
#include <vector>
#include "../include/ast.h"

using namespace std;

extern int yylex();
extern int yyparse();

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE cmd_scan_string(const char * str); // it does not work.
extern YY_BUFFER_STATE cmd_scan_buffer(char *, size_t);
extern void cmd_delete_buffer(YY_BUFFER_STATE buffer);
extern void cmd_switch_to_buffer(YY_BUFFER_STATE buffer);



int cmderror(const char * p)
{
    cerr << p << endl;
}

std::vector<Node*> nodes;


%}

%define api.prefix {cmd}


%union {
    int intval;
    double floatval;
};

%token<floatval> FLOAT
%token<intval>   INT
%token IDENT

%token T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_OPAREN T_CPAREN T_QUIT

%type<floatval> float_expr
%type<intval> int_expr


%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%%

program:                
    line            
    | program line 
    ;

line:
    float_expr  { nodes.push_back(new Literal<double>($1)); }
    | int_expr  { nodes.push_back(new Literal<int>($1)); }
    | command
    | error 
    ;

command:
    T_QUIT      { nodes.push_back(new CmdQuit()); }
    ;

float_expr:                           
    FLOAT                                   { $$ = $1; }
    | float_expr T_PLUS float_expr          { $$ = $1 + $3; }
    | float_expr T_MINUS float_expr         { $$ = $1 - $3; }
    | float_expr T_MULTIPLY float_expr      { $$ = $1 * $3; }
    | float_expr T_DIVIDE float_expr        { $$ = $1 / $3; }
    | T_OPAREN float_expr T_CPAREN          { $$ = $2; }
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
    | T_OPAREN int_expr T_CPAREN            { $$ = $2; }
    | T_PLUS int_expr                       { $$ = $2; }
    | T_MINUS int_expr                      { $$ = -$2; }
    ;



%%

Result eval(std::string line)
{
    YY_BUFFER_STATE buffer = cmd_scan_string(line.c_str());
    cmd_switch_to_buffer(buffer);
    int rv = cmdparse();

    Result result(rv);
    result.AddNodes(nodes);
    nodes.clear();
    cmd_delete_buffer(buffer);
    return result;
}

void clean_up(Result result)
{
    for (auto node : result.getNodes())
    {
        delete node;
    }
}

