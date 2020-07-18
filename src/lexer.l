
%{
  #include <iostream>
  #include <sstream>
  #include "parser.h"
  using namespace std;
  extern int yylex();

  template <typename T>
  T parse_number(std::string numstr)
  {
    stringstream ss;
    ss << numstr;
    T result(0);
    ss >> result;
    return result;
  }


%}

FLOAT     [0-9]+\.{1}[0-9]+([e|E]{1}[+|-]?[0-9]+)?
INT       [0-9]+
IDENT     [a-zA-Z0-9]+
QUIT      "quit"

%option noyywrap

%%

[ \t\n]           ;
{FLOAT}         { yylval.floatval = parse_number<double>(yytext);  return FLOAT; }
{INT}           { yylval.intval = parse_number<int>(yytext);      return INT; }

{QUIT}          { return T_QUIT; }

{IDENT}         { return IDENT; }
"+"             { return T_PLUS; }
"-"             { return T_MINUS; }
"/"             { return T_DIVIDE; }
"*"             { return T_MULTIPLY; }
"("             { return T_OPAREN; }
")"             { return T_CPAREN; }
%%


