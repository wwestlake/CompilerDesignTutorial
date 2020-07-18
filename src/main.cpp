

#include <iostream>
#include <string>
#include "ast.h"

extern int yylex(); 
extern int yyparse();


extern Result eval(std::string line);
extern void clean_up(Result result);

void print_prompt(std::string prompt)
{
    std::cout << prompt;
    std::cout.flush();
}

std::string read_line()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

Result eval_command(std::string line)
{
    return eval(line);
}

int main(int argc, char ** argv)
{
    std::cout << "Welcome to Dungeon Crawl 2" << std::endl;
    Context context;
    context.running = true;

    while (context.running) {
        print_prompt("Dungeon> ");
        std::string line = read_line();
        if (line.length() > 0)
        {
            Result result =  eval_command(line);

            for (auto node : result.getNodes())
            {
                node->Execute(context);
            }
            clean_up(result);
        }

    }

    std::cout << "Good Bye!" << std::endl;

}
