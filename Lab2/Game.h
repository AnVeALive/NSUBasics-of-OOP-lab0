#include <iostream>
#include <fstream>
#include <set>
#include "Grid.h"

const int MID_HEIGHT = 12;
const int MID_WIDTH  = 25;
const int HEIGHT     = 25;
const int WIDTH      = 50;

class Game {
public:
    std::set<int> _birth;
    std::set<int> _survival;
    Grid _grid;
    int _iteration;

public:
    Game();

    void loadFromFile(const std::string &filename);
    void saveToFile(const std::string &filename);

    void tick(const int n);
    void showHelp() const;

    void setRules(const std::string &ruleLine);
    void setGridSize(const std::string &sizeLine);

    void printGrid();
};