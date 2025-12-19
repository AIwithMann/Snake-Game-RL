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
    char last_move;
    std::array<int,2> appleIdx;
    std::uniform_int_distribution<int> dist_rows;
    std::uniform_int_distribution<int> dist_cols;
    std::mt19937 gen;

    void spawnFood();

public:
    Snake(int rows, int cols);
    bool getInput(char& c); // Input handling (Linux/macOS version)
    bool move(char m);
    void clearScreen();
    void print() const;
    void iterate();
    int headDirection() const;
    int getHeight() const;
    int getWidth() const;
    bool collision() const;
    std::array<int,2> getAppleIdx() const;
    const std::array<int,2> getHeadIndex() const;
    bool in(int x, int y) const;
    void reset();
    void enableRawMode();
    int getLength();
};
