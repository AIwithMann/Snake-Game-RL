#pragma once
#include "Snake.h"
#include<iostream>
#include<array>
#include<vector>
#include<cstdint>
#include<stdexcept>
using FEATURE = std::array<uint8_t, 12>;
using WEIGHT = std::array<float, 12 * 4>;
using STATE = std::array<int,2>;
using ACTION = int;

class Agent{
private:
    int NUM_FEATURES = 12;
    int NUM_ACTIONS = 4;

    Snake snake;
    FEATURE feature;
    WEIGHT weights;

    float lr;
    float gamma;
    float eps;
    int steps;
    int maxIter;

    FEATURE getFeature(const STATE& s) const;
    float getQ(FEATURE& s, ACTION a) const;
    int reward(const STATE& prevS) const;
public:
    Agent(const Snake& _snake, float learningRate, float gamma, float epsilon, int n, int maxIteration);
    ACTION chooseAction(STATE s);
    void train();
    void play();
};