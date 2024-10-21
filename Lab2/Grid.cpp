#include <iostream>
#include "Grid.h"



Cell::Cell() : _alive(false), _change(false) {}

bool Cell::isAlive() {
    return _alive;
}

bool Cell::isChange() {
    return _change;
}

void Cell::setAlive(const bool alive) {
    _alive = alive;
}

void Cell::setChange(const bool change) {
    _change = change;
}



Grid::Grid() {}

Grid::Grid(int height, int width) : _height(height), _width(width),
    _cells(height, std::vector<Cell>(width)) {
}

int Grid::getHeight() const {
    return _height;
}

int Grid::getWidth() const {
    return _width;
}

int Grid::countNeighbors(int y, int x) {
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

bool Grid::isAlive(const int y, const int x) {
    return _cells[y][x].isAlive();
}

bool Grid::isChange(const int y, const int x) {
    return _cells[y][x].isChange();
}

void Grid::setAlive(int y, int x, bool alive) {
    _cells[y][x].setAlive(alive);
}

void Grid::setChange(int y, int x, bool change) {
    _cells[y][x].setChange(change);
}

void Grid::print() {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            std::cout << ((_cells[i][j].isAlive()) ? 'o' : ' ');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}