
%{
  #include <iostream>
  #include <sstream>
  #include <vector>
  #include <string>
  #include <string.h>
  #include <stdio.h>
  #include "../include/dungeon.h"
  #include "dungeon_parser.hpp"

extern int dnglex();

static std::vector<char>  string_buffer;

static std::string getString(const std::vector<char> buf)
{
    std::stringstream ss;
    for (auto ch : buf)
    {
        ss << ch;
    }
    return ss.str();
}

static void clear_buffer(std::vector<char>& buf)
{
    buf.clear();
}

static void append_char(std::vector<char>& buf, char ch)
{
    buf.push_back(ch);
}

static void append_str(std::vector<char>& buf, char* str)
{
    std::string as_str(str);
    for (auto ch : as_str)
    {
        buf.push_back(ch);
    }
}

template <typename T>
static T parse_number(std::string numstr)
{
    std::stringstream ss;
    ss << numstr;
    T result(0);
    ss >> result;
    return result;
}



%}
%option prefix="dng"
%option noyywrap



FLOAT     [0-9]+\.{1}[0-9]+([e|E]{1}[+|-]?[0-9]+)?
INT       [0-9]+
IDENT     [a-zA-Z0-9_]+
DESC      description|desc
EOL [\n\r]
ALL (.|\n)

%x str_start
%x b_comment
%x l_comment
%x b_text

%%

[ \t\n\r]       ;


\"                          { BEGIN str_start; clear_buffer(string_buffer);  }
<str_start>[^\\"\n]*        { append_str(string_buffer, dngtext);  } 
<str_start>\\a              { append_char(string_buffer, '\a'); }
<str_start>\\b              { append_char(string_buffer, '\b'); }
<str_start>\\e              { append_char(string_buffer, '\e'); }
<str_start>\\f              { append_char(string_buffer, '\f'); }
<str_start>\\n              { append_char(string_buffer, '\n'); }
<str_start>\\r              { append_char(string_buffer, '\r'); }
<str_start>\\v              { append_char(string_buffer, '\v'); }
<str_start>\\t              { append_char(string_buffer, '\t'); }
<str_start>\\[\\"]          { append_char(string_buffer, dngtext[1]); }
<str_start>\"               { dnglval.str = strdup(getString(string_buffer).c_str()); 
                                    BEGIN 0; return T_STRING; }


"(*"                        { BEGIN b_comment; }
<b_comment>"*)"             { BEGIN 0; }
<b_comment>{EOL}            ;  
<b_comment>{ALL}            ;

"#"                         { BEGIN l_comment; }
<l_comment>.*            ;
<l_comment>{EOL}            { BEGIN 0; }

"@\""                       { BEGIN b_text; clear_buffer(string_buffer); }
<b_text>\\[\\"]             { append_char(string_buffer, dngtext[1]); }
<b_text>"\""                { dnglval.str = strdup(getString(string_buffer).c_str());   BEGIN 0; return T_STRING; } 
<b_text>{ALL}               { append_char(string_buffer, dngtext[0]); }

{INT}                       { dnglval.intval = parse_number<int>(yytext); return INT; }


";"                         { return ';'; }  
"["                         { return '['; }
"]"                         { return ']'; }
","                         { return ','; }
":"                         { return ':'; }
"("                         { return '('; }    
")"                         { return ')'; }    
"{"                         { return '{'; }
"}"                         { return '}'; }
"="                         { return '='; }


"Dungeon"                   { return DUNGEON; }
"Room"                      { return ROOM;    }
"DisplayName"               { return NAME;    }
"Description"               { return DESC;    }
"Exits"                     { return EXITS;   }    
"Unique"                    { return UNIQUE;  }
"Item"                      { return ITEM;    }
"Value"                     { return VALUE;   }
"Text"                      { return TEXT;    }
"Items"                     { return ITEMS;   }
"start"                     { return START;   }
{IDENT}                     { dnglval.str = strdup(dngtext); return IDENT;   }

%%


