#include <iostream>
#include <sstream>
#include "Game.h"

int main(int argc, char *argv[]) {
    Game game;

    if (argc > 1) {
        if (argv[1][0] != '-') {
            game.loadFromFile(argv[1]);
        }
    }

    std::string arg;
    for (int i = 1; i < argc; i++) {
        arg = argv[i];

        if (arg == "-i") {
            game.tick(std::stoi(argv[i + 1]));
        } else if (arg.rfind("--iteration=", 0) == 0) {
            game.tick(std::stoi(arg.substr(arg.find('=') + 1)));
        }

        if (arg == "-o") {
            game.saveToFile((std::string)argv[i + 1]);
            return 0;
        } else if (arg.rfind("--output=", 0) == 0) {
            game.saveToFile(arg.substr(arg.find('=') + 1));
            return 0;
        }
    }

    game.printGrid();

    std::string cmd;
    while (true) {
        getline(std::cin, cmd);

        if (cmd == "exit") {
            return 0;
        } else if (cmd == "help") {
            game.showHelp();
        } else if (cmd.rfind("dump", 0) == 0) {
            std::string filename = cmd.substr(5);
            game.saveToFile(filename);
        } else if (cmd.rfind("tick", 0) == 0) {
            int iteration = 1;
            std::istringstream inss(cmd.substr(cmd.find(' ') + 1));
            inss >> iteration;
            game.tick(iteration);
            game.printGrid();
        } else {
            std::cout << "Для просмотра справки о командах введити: help" << std::endl;
        }
    }
}