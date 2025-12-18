#include "agent.h"
#include <cmath>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <algorithm>

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
    snake(_snake)
{
    weights.fill(0.0f);
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
    else if (ax > hx) f[5] = 1;
    else if (ay > hy) f[6] = 1;
    else if (ax < hx) f[7] = 1;

    f[8 + snake.headDirection()] = 1;
    return f;
}

ACTION Agent::chooseAction(FEATURE& s){
    float p = distEps(gen);
    if (p < eps) return distAction(gen);
    return argmax({getQ(s, 0), getQ(s,1), getQ(s, 2), getQ(s,3)});
}

void Agent::train(){
    for(int episode = 0; episode < maxIter; episode++){
        std::vector<FEATURE> states;
        std::vector<ACTION> actions;
        std::vector<float> rewards;
        rewards.push_back(0);
        snake.reset();
        currentState = snake.getHeadIndex();
        FEATURE current_feature = getFeature(currentState);
        actions.push_back(chooseAction(current_feature));
        states.push_back(current_feature);

        int t = 0, T = std::numeric_limits<float>::max();

        while(true){
            if(t < T){
                char m;
                ACTION a = chooseAction(states[states.size()-1]);

                if (a == 0) m = 'w';
                else if (a == 1) m = 'd';
                else if (a == 2) m = 's';
                else if (a == 3) m = 'a';
                STATE apple = snake.getAppleIdx();
                bool success = snake.move(m);
                STATE currentState = snake.getHeadIndex();
                FEATURE currentFeature = getFeature(currentState);
                states.push_back(current_feature);
                rewards.push_back(reward(currentState,apple));

                if(!success) T = t+1;
                else actions.push_back(a);

                int tau = t - steps + 1;
                if (tau >= 0){
                    float G = 0;
                    for(int i = tau + 1; i <= std::min(tau + steps, T); i++)
                        G += std::pow(gamma, i - tau - 1) * rewards[i];
                    
                    if (tau + steps < T){
                        FEATURE sTauN = states[tau+steps];
                        ACTION aTauN = actions[tau+steps];
                        G += std::pow(gamma, steps) * getQ(sTauN, aTauN);
                    }
                    FEATURE sTau = states[tau];
                    ACTION aTau = actions[tau];
                    float qTau = getQ(sTau, aTau);
                    float td = G - qTau;

                    int offset = aTau * NUM_FEATURES;
                    for(int featureIdx = 0; featureIdx < 12; featureIdx++)
                        weights[offset + featureIdx] += alpha * td * sTau[featureIdx];
                }
                if(tau == T - 1) break;
                t += 1;
            }
            std::cout<<episode;
        }
        std::cout << episode << std::endl;
    }
}
