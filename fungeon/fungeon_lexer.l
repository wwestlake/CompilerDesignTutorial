%{

#include "utils.h"
#include "fungeon_ast.h"
#include "fungeon_parser.hpp"


#define YY_USER_ACTION  fungeon_lloc.last_column += yyleng;

     /* Initialize LOC. */
     # define LOCATION_RESET(Loc)                  \
       (Loc).first_column = (Loc).first_line = 1;  \
       (Loc).last_column =  (Loc).last_line = 1;
     
     /* Advance of NUM lines. */
     # define LOCATION_LINES(Loc, Num)             \
       (Loc).last_column = 1;                      \
       (Loc).last_line += Num;
     
     /* Restart: move the first cursor to the last position. */
     # define LOCATION_STEP(Loc)                   \
       (Loc).first_column = (Loc).last_column;     \
       (Loc).first_line = (Loc).last_line;
     
     /* Output LOC on the stream OUT. */
     # define LOCATION_PRINT(Out, Loc)                               \
       if ((Loc).first_line != (Loc).last_line)                      \
         fprintf (Out, "%d.%d-%d.%d",                                \
                  (Loc).first_line, (Loc).first_column,              \
                  (Loc).last_line, (Loc).last_column - 1);           \
       else if ((Loc).first_column < (Loc).last_column - 1)          \
         fprintf (Out, "%d.%d-%d", (Loc).first_line,                 \
                  (Loc).first_column, (Loc).last_column - 1);        \
       else                                                          \
         fprintf (Out, "%d.%d", (Loc).first_line, (Loc).first_column)


static std::vector<char> strbuf;


%}
%option prefix="fungeon_"
%option noyywrap


HEX "0"[xX][0-9A-Fa-f]+
OCT "0"[oO][0-7]+
BIN "0"[bB][0-1]+
EOL [\n\r]
FLT [0-9]+\.[0-9]*(([e|E][+|-]?)[0-9]+)?
INT [0-9]+
SYM [a-z]{1}[a-zA-Z_]*[a-zA-Z0-9_]*
TYPE_SYM [A-Z]{1}[a-zA-Z_]*[a-zA-Z0-9_]*

BOOL "true"|"false"
SCOPE_OP "."|"::"
COLON ":"
QUEST "?"

%x string_st
%x char_st
%x b_comment
%x l_comment


%%


\"                     { BEGIN string_st; ClearBuffer(strbuf); }
<string_st>[^\\"\n]*   { AppendBufferString(strbuf, yytext); }

<string_st>\\a         { AppendBufferChar(strbuf, '\a'); }
<string_st>\\b         { AppendBufferChar(strbuf, '\b'); }
<string_st>\\e         { AppendBufferChar(strbuf, '\e'); }
<string_st>\\f         { AppendBufferChar(strbuf, '\f'); }
<string_st>\\n         { AppendBufferChar(strbuf, '\n'); }
<string_st>\\r         { AppendBufferChar(strbuf, '\r'); }
<string_st>\\v         { AppendBufferChar(strbuf, '\v'); }
<string_st>\\\\        { AppendBufferChar(strbuf, '\\'); }

<string_st>\\t         { AppendBufferChar(strbuf, '\t'); }

<string_st>\\[0-7]*    { AppendBufferChar(strbuf, strtol(yytext+1, 0, 8)); }
<string_st>\\[\\"]     { AppendBufferChar(strbuf, yytext[1]); }
<string_st>\"          { fungeon_lval.string_val = new std::string(strdup(getString(strbuf).c_str())); BEGIN 0; return STRING_VAL; }
<string_st>\\.         { fungeon_lval.error_message = error("bogus escape '%s' in string", yytext); return ERROR; }
<string_st>\n          { fungeon_lval.error_message = error("newline in string"); return ERROR; }


\'                     { BEGIN char_st; ClearBuffer(strbuf); }
<char_st>[^\\'\n]{1}   { AppendBufferChar(strbuf, yytext[0]); }
<char_st>\\a         { AppendBufferChar(strbuf, '\a'); }
<char_st>\\b         { AppendBufferChar(strbuf, '\b'); }
<char_st>\\e         { AppendBufferChar(strbuf, '\e'); }
<char_st>\\f         { AppendBufferChar(strbuf, '\f'); }
<char_st>\\n         { AppendBufferChar(strbuf, '\n'); }
<char_st>\\r         { AppendBufferChar(strbuf, '\r'); }
<char_st>\\v         { AppendBufferChar(strbuf, '\v'); }
<char_st>\\\\        { AppendBufferChar(strbuf, '\\'); }
<char_st>\\t         { AppendBufferChar(strbuf, '\t'); }
<char_st>\\[0-7]*    { AppendBufferChar(strbuf, strtol(yytext+1, 0, 8)); }
<char_st>\\[\\']     { AppendBufferChar(strbuf, yytext[1]); }
<char_st>\'          { fungeon_lval.char_val = strbuf[0]; BEGIN 0; return CHAR_VAL; }
<char_st>\\.         { fungeon_lval.error_message = error("bogus escape '%s' in character", yytext); return ERROR; }
<char_st>\n          { fungeon_lval.error_message = error("newline in character"); return ERROR; }


"/*"                { BEGIN b_comment; }
<b_comment>"*/"     { BEGIN 0; } 
<b_comment>{EOL}    { LOCATION_LINES(fungeon_lloc, fungeon_leng);}  
<b_comment>.*       {}
"//"                { BEGIN l_comment; }
<l_comment>{EOL}    { BEGIN 0; LOCATION_LINES(fungeon_lloc, fungeon_leng); }        
<l_comment>.*       {}


{HEX}               { fungeon_lval.inum_val = parse_number_hob(yytext); return INT_VAL; }
{OCT}               { fungeon_lval.inum_val = parse_number_hob(yytext); return INT_VAL; }
{BIN}               { fungeon_lval.inum_val = parse_number_hob(yytext); return INT_VAL; }
{INT}               { fungeon_lval.inum_val = parse_number<int>(yytext); return INT_VAL; }
{FLT}               { fungeon_lval.fnum_val = parse_number<double>(yytext); return FLOAT_VAL; }
{BOOL}              { fungeon_lval.bool_val = parse_bool(yytext); return BOOL_VAL; }


"let"               { return LET;       }     
"if"                { return IF;        }
"then"              { return THEN;      }
"else"              { return ELSE;      }
"lambda"            { return LAMBDA;    }
"type"              { return TYPE;    }
"int"               { return INT_T;     }
"float"             { return FLOAT_T;   }
"string"            { return STRING_T;  }
"char"              { return CHAR_T;    }
"byte"              { return BYTE_T;    }
"bool"              { return BOOL_T;    }
"print"             { return PRINT_T; }
"with"              { return WITH; }
"of"                { return OF; }
"record"            { return RECORD; }
"enumerate"         { return ENUMERATION; }
"list"              { return LIST; }
"tuple"             { return TUPLE; }

"++"                { return INCREMENT; }
"--"                { return DECREMENT; }
"+"                 { return PLUS; }
"*"                 { return TIMES; }
"/"                 { return DIVIDE; }
"%"                 { return MOD; }
"=="                { return EQ; }
"="                 { return ASSIGN; }
"!="                { return NOT_EQ; }
"!"                 { return NOT; }
"->"                { return LAMBDA; }
"-"                 { return MINUS; }
"=>"                { return INTO; }
">>="               { return BIND; }
">=>"               { return KLEISLY_BIND; }
">>"                { return COMPOSE; }
"|>"                { return BEFORE; }
"<|"                { return AFTER; }
">="                { return GT_EQ; }
"<="                { return LT_EQ; }
">"                 { return GT; }
"<"                 { return LT; }
"|"                 { return PIPE; }
"->"                { return RIGHT_ARROW; }


{SCOPE_OP}          { fungeon_lval.op_val = yytext; return SCOPE_OP; }
{COLON}             { return COLON; }
{QUEST}             { return QUEST; }

"()"                { return UNIT; }
"("                 { return OPAREN; }
")"                 { return CPAREN; }
"{"                 { return OBRACE; }
"}"                 { return CBRACE; }
","                 { return COMMA; }
";"                 { return SEMI_COLON; }
"["                 { return OBRACKET; }
"]"                 { return CBRACKET; }

{TYPE_SYM}          { fungeon_lval.symbol = strdup( yytext ); return TYPE_IDENT; }
{SYM}               { fungeon_lval.symbol = strdup( yytext ); return IDENT; }



{EOL}               { LOCATION_LINES(fungeon_lloc, fungeon_leng);  }
[ \t]+              { LOCATION_STEP (fungeon_lloc); }

.                   { 
                        fungeon_lval.error_message = error("In column: %d, invalid character: %s", fungeon_lloc.first_column, yytext );
                        LOCATION_STEP (fungeon_lloc);
                        return ERROR;
                    }

<<EOF>>             { return 0; }



%%


