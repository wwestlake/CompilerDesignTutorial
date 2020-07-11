
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

%option noyywrap

%%

[ \t]           ;
[ \n]           { return T_NEWLINE; }
[+-]?[0-9]+\.[0-9]+  { yylval.floatval = parse_number<float>(yytext);  return FLOAT; }
[+-]?[0-9]+          { yylval.intval = parse_number<int>(yytext);      return INT; }
[a-zA-Z0-9]+    { return IDENT; }
"+"             { return T_PLUS; }
"-"             { return T_MINUS; }
"/"             { return T_DIVIDE; }
"*"             { return T_MULTIPLY; }
"("             { return T_OPAREN; }
")"             { return T_CPAREN; }
%%


