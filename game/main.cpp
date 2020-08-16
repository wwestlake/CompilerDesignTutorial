

#include <iostream>
#include <string>
#include <memory>
#include "../include/ast.h"
#include "../include/dungeon.h"

extern int cmdlex(); 
extern int cmdparse();

extern std::tuple<int, Dungeon*> dng_eval(std::string filename);


extern Result eval(std::string line, Dungeon* dungeon);
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

Result eval_command(std::string line, Dungeon* dungeon)
{
    return eval(line, dungeon);
}

int command_line(Dungeon* dungeon)
{
    std::cout << "Welcome to Dungeon Crawl 2" << std::endl;
    Context context;
    context.running = true;

    std::string playerName;
    std::cout << "Enter your player name: ";
    std::cout.flush();

    playerName = read_line();

    context.dungeon = std::shared_ptr<Dungeon>(dungeon);
    context.player = std::make_shared<Player>(playerName, "lobby");


    while (context.running) {
        print_prompt(context.player->getPLayerName() + ": " + context.player->getCurrentRoom() + "> ");
        std::string line = read_line();
        if (line.length() > 0)
        {
            Result result =  eval_command(line, dungeon);

            for (auto node : result.getNodes())
            {
                node->Execute(context);
            }
            clean_up(result);
        }

    }

    std::cout << "Good Bye!" << std::endl;
    return 0;
}


int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        std::cout << "You must specify a file" << std::endl;
        return EXIT_FAILURE;
    }

    auto [ result, dung ] = dng_eval(argv[1]);

    if (result == 0 && dung != nullptr) {
        command_line(dung);
    } else {
        std::cout << "There were errors, game exiting" << std::endl;
    }

    return 0;
}
