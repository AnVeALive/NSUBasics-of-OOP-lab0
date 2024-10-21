#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

const int MID_HEIGHT = 12;
const int MID_WIDTH  = 25;
const int HEIGHT     = 25;
const int WIDTH      = 50;

class Cell {
private:
    bool _alive;
    bool _change;

public:
    Cell() : _alive(false), _change(false) {}

    bool isAlive() {
        return _alive;
    }

    bool isChange() {
        return _change;
    }

    void setAlive(const bool alive) {
        _alive = alive;
    }

    void setChange(const bool change) {
        _change = change;
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

    int getHeight() const {
        return _height;
    }

    int getWidth() const {
        return _width;
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

    bool isAlive(const int y, const int x) {
        return _cells[y][x].isAlive();
    }

    void setAlive(int y, int x, bool alive) {
        _cells[y][x].setAlive(alive);
    }

    bool isChange(const int y, const int x) {
        return _cells[y][x].isChange();
    }

    void setChange(int y, int x, bool change) {
        _cells[y][x].setChange(change);
    }
    
    void print() {
        for (int i = 0; i < _height; i++) {
            for (int j = 0; j < _width; j++) {
                std::cout << ((_cells[i][j].isAlive()) ? 'o' : ' ');
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
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

    void loadFromFile(const std::string &filename) {
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

    void saveToFile(const std::string &filename) {
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

    void tick(const int n) {
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

    void showHelp() const {
        std::cout << "+-----+ Справка о командах +-----+" << std::endl;
        std::cout << "dump <filename> - сохранить вселенную в файл." << std::endl;
        std::cout << "tick <n> - расчитать n-ое поколение." << std::endl;
        std::cout << "exit - выход из игры." << std::endl;
        std::cout << "help - справка о командах" << std::endl << std::endl;
    }

    void setRules(const std::string &ruleLine) {
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

    void setGridSize(const std::string &sizeLine) {
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
            std::ifstream inss(cmd.substr(cmd.find(' ') + 1));
            inss >> iteration;
            game.tick(iteration);
        }
    }

    return 0;
}