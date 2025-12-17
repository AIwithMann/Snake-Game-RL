#include "agent.h"

Agent::Agent(const Snake& _snake, float learningRate, float gamma, float epsilon, int n, int maxIteration): 
    lr(learningRate),
    gamma(gamma),
    eps(epsilon),
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

int Agent::reward(const STATE& sPrime) const{
    bool ate = (sPrime == snake.getAppleIdx());
    bool collide = (sPrime[0] < 0 || sPrime[1] < 0 || sPrime[0] >= snake.getHeight() || sPrime[1] >= snake.getWidth() || snake.collision());
    if (ate) return 20;
    if (collide) return -20;
    return -1;
}

FEATURE Agent::getFeature(const STATE& s) const{
    FEATURE f{};
    auto body = snake.getSnake();
    int hx = body[0][0], hy = body[0][1];

    if(hx - 1 < 0 || snake.in(hx-1, hy)) f[0] = 1;
    if(hy - 1 < 0 || snake.in(hx, hy-1)) f[1] = 1;
    if(hx + 1 >= snake.getHeight() || snake.in(hx+1, hy)) f[2] = 1;
    if(hy + 1 >= snake.getWidth() || snake.in(hx, hy+1))f[3] = 1;

    auto apple = snake.getAppleIdx();
    int ax = apple[0], ay = apple[1];

    if (ay < hy)
        f[4] = 1;
    else if (ax > hx)
        f[5] = 1;
    else if (ay > hy)
        f[6] = 1;
    else if (ax < hx)
        f[7] = 1;

    f[8 + snake.headDirection()] = 1;
    return f;
}