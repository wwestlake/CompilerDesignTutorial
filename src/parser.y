%{
#include <iostream>

using namespace std;

extern int yylex();
extern int yyparse();

int yyerror(const char * p)
{
    cerr << p << endl;
}


%}



%union {
    int intval;
    float floatval;
};

%token<floatval> FLOAT
%token<intval>   INT
%token IDENT

%token T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_OPAREN T_CPAREN T_NEWLINE

%type<floatval> expr

%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%%

program:                
    line            
    | program line 
    ;

line:
    expr T_NEWLINE { cout << "The answer is: " << $1 << endl; }
    ;

expr:                           
    FLOAT                       { $$ = $1; }
    | INT                       { $$ = $1; }
    | expr T_PLUS expr          { $$ = $1 + $3; }
    | expr T_MINUS expr         { $$ = $1 - $3; }
    | expr T_MULTIPLY expr      { $$ = $1 * $3; }
    | expr T_DIVIDE expr        { $$ = $1 / $3; }
    | T_OPAREN expr T_CPAREN    { $$ = $2; }

    ;

%%

int main(int argc, char ** argv)
{
    cout << "Starting lexer" << endl;

    yyparse();

    return 0;
}

