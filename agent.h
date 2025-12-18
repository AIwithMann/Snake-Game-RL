#pragma once
#include "Snake.h"
#include<iostream>
#include<array>
#include<vector>
#include<cstdint>
#include<stdexcept>
#include<random>
#include<initializer_list>
#include<limits>
#include<cmath>
#include<algorithm>
using FEATURE = std::array<uint8_t, 12>;
using WEIGHT = std::array<float, 12 * 4>;
using STATE = std::array<int,2>;
using ACTION = int;

class Agent{
private:
    int NUM_FEATURES = 12;
    int NUM_ACTIONS = 4;

    Snake& snake;
    FEATURE feature;
    WEIGHT weights;
    STATE currentState;

    float lr;
    float gamma;
    float eps;
    float alpha;
    int steps;
    int maxIter;

    std::mt19937 gen;
    std::uniform_real_distribution<float> distEps{0.0f,1.0f};
    std::uniform_int_distribution<int> distAction{0,3};
    std::uniform_int_distribution<int> distRow{0,snake.getHeight() - 1};
    std::uniform_int_distribution<int> distCol{0, snake.getWidth() - 1};

    FEATURE getFeature(const STATE& s) const;
    float getQ(FEATURE& s, ACTION a) const;
    int reward(const STATE& sPrime, STATE appleIdx) const;

public:
    Agent(Snake& _snake, float learningRate, float gamma, float epsilon, float alpha, int n, int maxIteration);
    ACTION chooseAction(FEATURE& s);
    void train();
    void play();
};
