#include "Game.h"

Game::Game() : _birth{3}, _survival{2, 3}, _grid(HEIGHT, WIDTH), _iteration(0) {
    _grid.setAlive(MID_HEIGHT    , MID_WIDTH    , true);
    _grid.setAlive(MID_HEIGHT + 1, MID_WIDTH    , true);
    _grid.setAlive(MID_HEIGHT - 1, MID_WIDTH    , true);
    _grid.setAlive(MID_HEIGHT    , MID_WIDTH - 1, true);
    _grid.setAlive(MID_HEIGHT + 1, MID_WIDTH + 1, true);
}

void Game::loadFromFile(const std::string &filename) {
    std::ifstream fin(filename);

    if (!fin.is_open()) {
        throw std::ios_base::failure("The file could not be opened: " + filename);
    }

    _iteration = 0;

    std::string line;
    while (!fin.eof()) {
        getline(fin, line);

        if (line[1] == 'R') {
            setRules(line);
        }

        if (line[1] == 'S') {
            setGridSize(line);
        }

    }
    fin.close();
}

void Game::saveToFile(const std::string &filename) {
    std::ofstream fout(filename);

    if (!fout.is_open()) {
        throw std::ios_base::failure("The file could not be opened: " + filename);
    }

    fout << "#Life 1.06" << std::endl << "#R B";
    for (int elem : _birth) {
        fout << (char)(elem + '0');
    }
    fout << "/S";
    for (int elem : _survival) {
        fout << (char)(elem + '0');
    }
    fout << std::endl;

    fout << "#S H" << _grid.getHeight() << "/";
    fout << "W" << _grid.getWidth() << std::endl;

    std::cout << "The universe was successfully saved.\n" << std::endl;
    fout.close();
}

void Game::tick(const int n) {
    for (int i = 0; i < n; i++) {
        _iteration++;
        for (int y = 0; y < _grid.getHeight(); y++) {
            for (int x = 0; x < _grid.getWidth(); x++) {
                if (_grid.isAlive(y, x)) {
                    if (_survival.find(_grid.countNeighbors(y, x)) ==
                        _survival.end()) {
                        _grid.setChange(y, x, true);
                    }
                } else {
                    if (_birth.find(_grid.countNeighbors(y, x)) !=
                        _birth.end()) {
                        _grid.setChange(y, x, true);
                    }
                }
            }
        }
        for (int y = 0; y < _grid.getHeight(); y++) {
            for (int x = 0; x < _grid.getWidth(); x++) {
                if (_grid.isChange(y, x)) {
                    if (_grid.isAlive(y, x)) {
                        _grid.setAlive(y, x, false);
                    } else {
                        _grid.setAlive(y, x, true);
                    }
                    _grid.setChange(y, x, false);
                }
            }
        }
    }
    printGrid();
}

void Game::showHelp() const {
    std::cout << "+-----+ Справка о командах +-----+" << std::endl;
    std::cout << "dump <filename> - сохранить вселенную в файл." << std::endl;
    std::cout << "tick <n> - расчитать n-ое поколение." << std::endl;
    std::cout << "exit - выход из игры." << std::endl;
    std::cout << "help - справка о командах" << std::endl << std::endl;
}

void Game::setRules(const std::string &ruleLine) {
    int b = ruleLine.find('B');
    int s = ruleLine.find('S');

    int i = 1;
    while (ruleLine[b + i] != '/') {
        _birth.insert(ruleLine[b + i] - '0');
        i++;
    }
    i = 1;
    while (ruleLine[s + i] != '\n' && s + i < ruleLine.size()) {
        _survival.insert(ruleLine[s + i] - '0');
        i++;
    }
}

void Game::setGridSize(const std::string &sizeLine) {
    int h = sizeLine.find('H');
    int w = sizeLine.find('W');

    int height = 0;
    int width = 0;
    int i = 1;
    while (sizeLine[h + i] != '/') {
        height = height * 10 + (sizeLine[h + i] - '0');
        i++;
    }
    i = 1;
    while (sizeLine[w + i] != '\n' && w + i < sizeLine.size()) {
        width = width * 10 + (sizeLine[w + i] - '0');
        i++;
    }
    _grid = Grid(height, width);
}

void Game::printGrid() {
    _grid.print();
}