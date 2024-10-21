#include <vector>

class Cell {
private:
    bool _alive;
    bool _change;

public:
    Cell();

    bool isAlive();
    bool isChange();

    void setAlive(const bool alive);
    void setChange(const bool change);
};

class Grid {
private:
    std::vector<std::vector<Cell>> _cells;
    int _height;
    int _width;

public:
    Grid();
    Grid(int height, int width);

    int getHeight() const;
    int getWidth() const;

    int countNeighbors(int y, int x);

    bool isAlive(const int y, const int x);
    bool isChange(const int y, const int x);

    void setAlive(int y, int x, bool alive);
    void setChange(int y, int x, bool change);

    void print();
};