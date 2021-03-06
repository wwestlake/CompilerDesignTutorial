
%{
  #include <iostream>
  #include <sstream>
  #include "parser.hpp"
  #include "../include/lex_utilities.h"

  using namespace std;
  extern int cmdlex();



%}

FLOAT     [0-9]+\.{1}[0-9]+([e|E]{1}[+|-]?[0-9]+)?
INT       [0-9]+
IDENT     [a-zA-Z0-9_]+
QUIT      "quit"
INFO      "info"
GO        "go"

%option prefix="cmd"
%option noyywrap


%%

[ \t\n]           ;
{FLOAT}         { cmdlval.floatval = parse_number<double>(yytext);  return FLOAT; }
{INT}           { cmdlval.intval = parse_number<int>(yytext);      return INT; }

{QUIT}          { return T_QUIT; }
{INFO}          { return T_INFO; }
{GO}            { return T_GO; }

{IDENT}         { cmdlval.ident = yytext; return IDENT; }
"+"             { return T_PLUS; }
"-"             { return T_MINUS; }
"/"             { return T_DIVIDE; }
"*"             { return T_MULTIPLY; }
"("             { return T_OPAREN; }
")"             { return T_CPAREN; }
%%


