#include "Snake.h"
#include "agent.h"

int main(){
    Snake game(10, 20);
    Agent snakeagent(game, 0.5, 0.9, 0.2, 0.1,5,50000);
    snakeagent.train();
    return 0;
}

