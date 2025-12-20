#include "Snake.h"
#include "agent.h"

int main(){
    Snake game(10, 20);
    Agent snakeagent(game,0.9, 0.2, 0.01,5,50000);
    snakeagent.train();
    snakeagent.play();
    snakeagent.saveWeights("weights.txt");
    return 0;
}

