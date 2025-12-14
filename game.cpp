#include<iostream>
#include<random>
#include<array>
#include<vector>
#include<stdexcept>
#include<utility>
#include<cstdlib>

std::random_device rd;
std::mt19937 gen(rd());
class Snake{
  private:
    int height,width;
    std::vector<std::vector<int>> grid;
    std::vector<std::array<int,2>> snake;
    int length;
    int appleIdx[2];
    std::uniform_int_distribution<int> dist_rows;
    std::uniform_int_distribution<int> dist_cols;

  public:
    Snake(int rows, int cols) : height(rows), width(cols),
                                grid(rows, std::vector<int>(cols, 0)),
                                length(1),
                                dist_rows(0, rows - 1),
                                dist_cols(0, cols - 1) {
        snake.push_back({height / 2, width / 2});  // start in middle
        spawnFood();
    }
    bool in(int x, int y){
      for(int i = 0; i < length; i++){
        if(snake[i][0] == x && snake[i][1] ==  y) 
          return true;
      }
      return false;
    }
    void spawnFood(){
      int x = dist_rows(gen), y = dist_cols(gen);
      while(1){
        if(!in(x,y) && grid[x][y] == 0){
          appleIdx[0] = x;
          appleIdx[1] = y;
          grid[x][y] = 1;
          break;
        }
        x = dist_rows(gen);
        y = dist_cols(gen);
      }
    }
    void clearScreen() {
      #ifdef _WIN32
          std::system("cls");
      #else
          std::system("clear");
      #endif
    }
    bool collision(){
      for(int pass = 0; pass < length; pass++){
        for(int i = 0; i < length && i != pass; i++){
          if(snake[pass] == snake[i])
            return true;
        }
      }
      return false;
    }
    bool move(char m){
      switch(m){
        case 'w':{
          if ((snake[0][0]-1)<0)
            return false;
          bool ate = (grid[snake[0][0]-1][snake[0][1]] == 1);
          auto tail = snake[length-1];
          for(int i = length-1; i > 0; i--){
            snake[i] = snake[i-1];
          }
          snake[0][0]--;
          if(ate){
            snake.push_back(tail);
            length++;
            grid[snake[0][0]][snake[0][1]] = 0;
            spawnFood();
          }
          if(collision())
            return false;
          return true;
        }
        case 's':{
          if((snake[0][0]+1) >= height)
            return false;
          bool ate = (grid[snake[0][0]+1][snake[0][1]] == 1);
          auto tail = snake[length - 1];
          for(int i = length-1; i > 0; i--){
            snake[i] = snake[i-1];
          }
          snake[0][0]++;
          if(ate){
            snake.push_back(tail);
            length++;
            grid[snake[0][0]][snake[0][1]] = 0;
            spawnFood();
          }
          if(collision())
            return false;
          return true;
        }
        case 'a':{
          if((snake[0][1]-1) < 0)
            return false;
          bool ate = (grid[snake[0][0]][snake[0][1]-1] == 1);
          auto tail = snake[length-1];
          for(int i = length - 1; i > 0; i--){
            snake[i] = snake[i-1];
          }
          snake[0][1]--;
          if(ate){
            snake.push_back(tail);
            length++;
            grid[snake[0][0]][snake[0][1]] = 0;
            spawnFood();
          }
          if(collision())
            return false;
          return true;
        }
        case 'd':{
          if((snake[0][1]+1) >= width)
            return false;
          bool ate = (grid[snake[0][0]][snake[0][1]+1] == 1);
          auto tail = snake[length-1];
          for(int i = length - 1; i > 0; i--){
            snake[i] = snake[i-1];
          }
          snake[0][1]++;
          if(ate){
            snake.push_back(tail);
            length++;
            grid[snake[0][0]][snake[0][1]] = 0;
            spawnFood();
          }
          if(collision())
            return false;
          return true;
        }
        default:{
          throw std::runtime_error("invalid input");
        }
      }
    }
    
    void print(){
      for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
          if(in(i,j)){
            std::cout << "█";
          }else if(i == appleIdx[0] && j == appleIdx[1]){
            std::cout << "*";
          }else{
            std::cout << "-";
          }
        }
        std::cout << std::endl;
      }
    }

    void iterate(){
      while(1){
        if(length == height*width){
          clearScreen();
          print();
          std::cout << "YOU WON!\n";
          break;
        }
        clearScreen();
        print();
        char c;
        std::cin >> c;
        if(move(c) == false){
          clearScreen();
          print();
          std::cout << "GAME OVER!\n";
          break;
        }
      }
    }
};
int main(){
  Snake game(10,10);
  game.iterate();
  return 0;
}
