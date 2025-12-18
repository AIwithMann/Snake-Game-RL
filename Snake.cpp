#include "Snake.h"
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static void enableRawMode() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

Snake::Snake(int rows, int cols)
    : height(rows), width(cols),
      grid(rows, std::vector<int>(cols, 0)),
      length(1),
      dist_rows(0, rows - 1),
      dist_cols(0, cols - 1),
      gen(std::random_device{}())
{
    snake.push_back({height / 2, width / 2});
    spawnFood();
}

bool Snake::getInput(char& c) {
    return read(STDIN_FILENO, &c, 1) == 1;
}

bool Snake::in(int x, int y) const{
    for (int i = 0; i < length; ++i) {
        if (snake[i][0] == x && snake[i][1] == y)
            return true;
    }
    return false;
}

void Snake::spawnFood(){
    int x = dist_rows(gen);
    int y = dist_cols(gen);
    while (true) {
        if (!in(x, y) && grid[x][y] == 0){
            appleIdx[0] = x;
            appleIdx[1] = y;
            grid[x][y] = 1;
            break;
        }
        x = dist_rows(gen);
        y = dist_cols(gen);
    }
}

void Snake::clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

bool Snake::collision() const{
    for (int pass = 0; pass < length; ++pass) {
        for (int i = 0; i < length; ++i) {
            if (i != pass && snake[pass] == snake[i])
                return true;
        }
    }
    return false;
}

bool Snake::move(char m) {
    switch (m) {
        case 'w': {
            if (snake[0][0] - 1 < 0) return false;
            bool ate = (grid[snake[0][0] - 1][snake[0][1]] == 1);
            auto tail = snake[length - 1];
            for (int i = length - 1; i > 0; --i) {
                snake[i] = snake[i - 1];
            }
            snake[0][0]--;
            if (ate) {
                snake.push_back(tail);
                ++length;
                grid[snake[0][0]][snake[0][1]] = 0;
                spawnFood();
            }
            return !collision();
        }
        case 's':{
            if (snake[0][0] + 1 >= height) return false;
            bool ate = (grid[snake[0][0] + 1][snake[0][1]] == 1);
            auto tail = snake[length - 1];
            for (int i = length - 1; i > 0; --i) {
                snake[i] = snake[i - 1];
            }
            snake[0][0]++;
            if (ate) {
                snake.push_back(tail);
                ++length;
                grid[snake[0][0]][snake[0][1]] = 0;
                spawnFood();
            }
            return !collision();
        }
        case 'a': {
            if (snake[0][1] - 1 < 0) return false;
            bool ate = (grid[snake[0][0]][snake[0][1] - 1] == 1);
            auto tail = snake[length - 1];
            for (int i = length - 1; i > 0; --i) {
                snake[i] = snake[i - 1];
            }
            snake[0][1]--;
            if (ate) {
                snake.push_back(tail);
                ++length;
                grid[snake[0][0]][snake[0][1]] = 0;
                spawnFood();
            }
            return !collision();
        }
        case 'd': {
            if (snake[0][1] + 1 >= width) return false;
            bool ate = (grid[snake[0][0]][snake[0][1] + 1] == 1);
            auto tail = snake[length - 1];
            for (int i = length - 1; i > 0; --i) {
                snake[i] = snake[i - 1];
            }
            snake[0][1]++;
            if (ate) {
                snake.push_back(tail);
                ++length;
                grid[snake[0][0]][snake[0][1]] = 0;
                spawnFood();
            }
            return !collision();
        }
        default:
            throw std::runtime_error("invalid input");
    }
}

void Snake::print() const {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (in(i, j)) {
                std::cout << "█";
            } else if (i == appleIdx[0] && j == appleIdx[1]) {
                std::cout << "*";
            } else {
                std::cout << "-";
            }
        }
        std::cout << std::endl;
    }
}

void Snake::iterate() {
    const int interval_ms = 500;
    last_move = 'd'; // default direction
    enableRawMode();
    while (true) {
        auto start = std::chrono::steady_clock::now();
        char c = '\0';
        if (getInput(c)) {
            last_move = c;
        }
        if (!move(last_move)) {
            clearScreen();
            print();
            std::cout << "GAME OVER!\n";
            break;
        }
        clearScreen();
        print();
        std::this_thread::sleep_until(start + std::chrono::milliseconds(interval_ms));
    }
}

int Snake::headDirection() const{
    switch(last_move){
        case 'w':
            return 0;
        case 'd':
            return 1;
        case 's':
            return 2;
        case 'a':
            return 3;
        default:
            return -1;
    }
}

void Snake::reset(){
    length = 1; 
    snake.clear();
    snake.push_back({height / 2, width / 2});
    spawnFood();
    last_move = 'd';
}
int Snake::getHeight() const{return height;}
int Snake::getWidth() const{return width;}
std::array<int, 2> Snake::getAppleIdx() const{return appleIdx;}

const std::array<int,2> Snake::getHeadIndex() const{return snake[0];}
