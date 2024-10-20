#include <iostream>
#include <fstream>
#include <vector>
#include <set>

const int MID_HEIGHT = 12;
const int MID_WIDTH  = 25;
const int HEIGHT     = 25;
const int WIDTH      = 50;

class Cell {
private:
    bool _alive;

public:
    Cell() : _alive(false) {}

    bool isAlive() {
        return _alive;
    }

    void setAlive(const bool alive) {
        _alive = alive;
    }
};

class Grid {
private:
    std::vector<std::vector<Cell>> _cells;
    int _height;
    int _width;

public:
    Grid() {}

    Grid(int height, int width) : _height(height), _width(width),
        _cells(height, std::vector<Cell>(width)) {
    }

    int countNeighbors(int y, int x) {
        int count = 0;
        int ny, nx;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dy == 0 && dx == 0) continue;

                ny = (y + dy + _height) % _height;
                nx = (x + dx + _width) % _width;
                if (_cells[ny][nx].isAlive()) {
                    count++;
                }
            }
        }
        return count;
    }

    void setAlive(int y, int x, bool alive) {
        _cells[y][x].setAlive(alive);
    }
    
    void print() {
        for (int i = 0; i < _height; i++) {
            for (int j = 0; j < _width; j++) {
                std::cout << ((_cells[i][j].isAlive()) ? '*' : '.');
            }
            std::cout << std::endl;
        }
    }
};

class Game {
public:
    std::set<int> _birth;
    std::set<int> _survival;
    Grid _grid;
    int _iteration;
    
public:
    Game() : _birth{3}, _survival{2, 3}, _grid(HEIGHT, WIDTH), _iteration(0) {
        _grid.setAlive(MID_HEIGHT    , MID_WIDTH    , true);
        _grid.setAlive(MID_HEIGHT + 1, MID_WIDTH    , true);
        _grid.setAlive(MID_HEIGHT - 1, MID_WIDTH    , true);
        _grid.setAlive(MID_HEIGHT    , MID_WIDTH - 1, true);
        _grid.setAlive(MID_HEIGHT + 1, MID_WIDTH + 1, true);
    }

    Game(const std::string &file) : _iteration(0) {
        std::ifstream fin(file);

        if (!fin.is_open()) {
            throw std::ios_base::failure("The file could not be opened: " + file);
        }

        std::string line;
        while (!fin.eof()) {
            getline(fin, line);

            if (line[1] == 'R') {
                int b = line.find('B');
                int s = line.find('S');

                int i = 1;
                while (line[b + i] != '/') {
                    _birth.insert(line[b + i] - '0');
                    i++;
                }
                i = 1;
                while (line[s + i] != '\n' && s + i < line.size()) {
                    _survival.insert(line[s + i] - '0');
                    i++;
                }
            }

            if (line[1] == 'S') {
                int h = line.find('H');
                int w = line.find('W');

                int height = 0;
                int width = 0;
                int i = 1;
                while (line[h + i] != '/') {
                    height = height * 10 + (line[h + i] - '0');
                    i++;
                }
                i = 1;
                while (line[w + i] != '\n' && w + i < line.size()) {
                    width = width * 10 + (line[w + i] - '0');
                    i++;
                }

                _grid = Grid(height, width);
            }

        }
        fin.close();
    }

    void loadFromFile(const std::string &file) {

    }

    void saveToFile(const std::string &file) {

    }

    void tick(int n = 1) {
        
    }

    void setRules(const std::string &rules) {
        _birth.clear();
        _survival.clear();

        int b = rules.find('B');
        int s = rules.find('S');

        int i = 0;
        while (rules[b + i] != '/') {
            _birth.insert(rules[b + i] - '0');
            i++;
        }
        i = 0;
        while (rules[s + i] != '\n' && s + i < rules.size()) {
            _survival.insert(rules[s + i]);
            i++;
        }
    }

    void help() {

    }

    void printGrid() {
        _grid.print();
    }
};

int main(int argc, char *argv[]) {
    Game game;

    if (argc > 1) {
        game.loadFromFile(argv[1]);
    }

    game.printGrid();

    return 0;
}