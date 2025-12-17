#pragma once
#include <array>
#include <vector>
#include <random>
#include <stdexcept>

class Snake {
private:
    int height, width;
    std::vector<std::vector<int>> grid;
    std::vector<std::array<int, 2>> snake;
    int length;
    std::array<int,2> appleIdx;
    std::uniform_int_distribution<int> dist_rows;
    std::uniform_int_distribution<int> dist_cols;
    std::mt19937 gen;

    bool in(int x, int y) const;
    void spawnFood();
    bool collision() const;

public:
    Snake(int rows, int cols);
    // Input handling (Linux/macOS version)
    bool getInput(char& c);
    bool move(char m);
    void clearScreen();
    void print() const;
    void iterate();
};
