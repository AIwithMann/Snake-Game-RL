#include "agent.h"
#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include<iostream>

int argmax(std::initializer_list<float> list) {
    if (list.size() == 0) {
        throw std::invalid_argument("argmax: empty list");
    }
    auto max_it = std::max_element(list.begin(), list.end());
    return std::distance(list.begin(), max_it);
}
Agent::Agent(Snake& _snake, float learningRate, float gamma, float epsilon,float alpha, int n, int maxIteration): 
    lr(learningRate),
    gamma(gamma),
    eps(epsilon),
    alpha(alpha),
    steps(n),
    maxIter(maxIteration),
    snake(_snake),
    distEps(0.0f, 1.0f),
    distAction(0,3),
    distRow(0,snake.getHeight() - 1),
    distCol(0, snake.getWidth() - 1)
{
    weights.fill(0.0f);
    gen = std::mt19937(std::random_device{}());
}


float Agent::getQ(FEATURE& s, ACTION a) const{
    if(a > 3 || a < 0) throw std::runtime_error("invalid action");
    int offset = a * 12;
    float q = 0;
    for(int i = 0; i < 12; i++){
        q += s[i] * weights[offset + i];
    }
    return q;
}

int Agent::reward(const STATE& sPrime, STATE appleIdx) const{
    bool ate = (sPrime == appleIdx);
    bool collide = (sPrime[0] < 0 || sPrime[1] < 0 || sPrime[0] >= snake.getHeight() || sPrime[1] >= snake.getWidth() || snake.collision());
    if (ate) return 20;
    if (collide) return -20;
    return -1;
}

FEATURE Agent::getFeature(const STATE& s) const{
    FEATURE f{};
    int hx = s[0], hy = s[1];

    if(hx - 1 < 0 || snake.in(hx-1, hy)) f[0] = 1;
    if(hy - 1 < 0 || snake.in(hx, hy-1)) f[1] = 1;
    if(hx + 1 >= snake.getHeight() || snake.in(hx+1, hy)) f[2] = 1;
    if(hy + 1 >= snake.getWidth() || snake.in(hx, hy+1))f[3] = 1;

    auto apple = snake.getAppleIdx();
    int ax = apple[0], ay = apple[1];

    if (ay < hy) f[4] = 1;
    if (ax > hx) f[5] = 1;
    if (ay > hy) f[6] = 1;
    if (ax < hx) f[7] = 1;

    f[8 + snake.headDirection()] = 1;
    return f;
}

ACTION Agent::chooseAction(FEATURE& s){
    float p = distEps(gen);
    if (p < eps) return distAction(gen);
    return argmax({getQ(s, 0), getQ(s,1), getQ(s, 2), getQ(s,3)});
}

void Agent::train() {
    for (int episode = 0; episode < maxIter; episode++) {
        std::cout << "episode" << episode << " ";

        snake.reset();
        STATE currentState = snake.getHeadIndex();
        FEATURE currentFeature = getFeature(currentState);

        ACTION currentAction = chooseAction(currentFeature);

        std::vector<FEATURE> featureHistory { currentFeature };
        std::vector<ACTION> actionHistory { currentAction };
        std::vector<float> rewardHistory; 

        int t = 0;
        int T = std::numeric_limits<int>::max(); // T is infinity initially
        const int limit = 2000; // Safety limit

        while (true) {
            if (t < T) {
                char moveChar;
                if (currentAction == 0) moveChar = 'w';
                else if (currentAction == 1) moveChar = 'd';
                else if (currentAction == 2) moveChar = 's';
                else moveChar = 'a';

                STATE apple = snake.getAppleIdx();
                
                bool success = snake.move(moveChar);

                STATE nextState = snake.getHeadIndex();
                FEATURE nextFeature = getFeature(nextState);
                float r = reward(nextState, apple);

                // Check for Step Limit (Starvation)
                if (t >= limit) {
                    r = -10.0f;      
                }

                bool terminal = !success || snake.collision();
                
                rewardHistory.push_back(r);
                
                featureHistory.push_back(nextFeature);

                if (terminal) {
                    T = t + 1; 
                } else {
                    currentAction = chooseAction(nextFeature);
                    actionHistory.push_back(currentAction);
                }
            }

            int tau = t - steps + 1;
            if (tau >= 0) {
                float G = 0.0f;
                int start = tau + 1;
                int end = std::min(tau + steps, T);
                
                for (int i = start; i <= end; ++i) {
                    G += std::pow(gamma, i - tau - 1) * rewardHistory[i - 1];
                }
                
                if (tau + steps < T) {
                    int bootstrap_t = tau + steps;
                    G += std::pow(gamma, steps) * getQ(featureHistory[bootstrap_t], actionHistory[bootstrap_t]);
                }

                FEATURE s_tau = featureHistory[tau];
                ACTION a_tau = actionHistory[tau];
                float q_tau = getQ(s_tau, a_tau);
                float td_error = G - q_tau;

                // Gradient Descent Update
                int offset = a_tau * 12;
                for (int i = 0; i < 12; i++) {
                    weights[offset + i] += alpha * td_error * s_tau[i];
                }
            }

            if (tau == T - 1) {
                break;
            }
            
            t++;
        }
        eps = std::max(0.01f, (1.0f - static_cast<float>(episode)/static_cast<float>(maxIter)));
        std::cout << "Length: " << snake.getLength() << "\n";
    }
}

void Agent::play(){
    char last_move = 'd';
    snake.enableRawMode();
    while(true){
        FEATURE current = getFeature(currentState);
        int move = chooseAction(current);
        if (move == 0) last_move = 'w';
        else if(move == 1) last_move = 'd';
        else if(move == 2) last_move = 's';
        else if(move == 3) last_move = 'a';

        if(!snake.move(last_move)){
            snake.clearScreen();
            snake.print();
            std::cout << "Game over";
            break;
        }
        snake.clearScreen();
        snake.print();
    }
}
