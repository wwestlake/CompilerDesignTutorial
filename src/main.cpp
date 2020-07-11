

#include <iostream>

extern int yylex(); 
extern int yyparse();

int main(int argc, char ** argv)
{
    std::cout << "Starting lexer" << std::endl;

    yyparse();

    return 0;
}
