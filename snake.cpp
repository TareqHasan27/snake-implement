#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#undef main

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 10;
int bonusOn = 0;
int bonusCreateTime, p_time;
TTF_Font* font = nullptr; 

class Snake {
public:
    Snake();
    void handleInput(SDL_Event &e);
    void move(SDL_Renderer *renderer);
    void render(SDL_Renderer *renderer);
    bool checkCollision();
    void grow();
    void spawnFood();
    void bonusfood();
    void generateObstacles();
    bool isFoodCollidingWithObstacles();
    bool checkcollisionbonus();
    void handlebonus();
    void playeatsound();
    void renderGameOver(SDL_Renderer *renderer);

    int score = 0;

private:
    
    SDL_Rect food;
    SDL_Rect bonus={-1,-1,0,0};
    vector<SDL_Rect> body;
    vector<SDL_Rect> obstacles;  
    int direction; 
    Mix_Chunk *eatsound;              
}; 
    Snake::Snake() {
    grow();
    direction = 3; // Start moving to the right
    generateObstacles();
    spawnFood(); 
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
    eatsound=Mix_LoadWAV("tom-scream.mp3");
    if (!eatsound) cout << SDL_GetError() << endl;
    }
     
    void Snake::generateObstacles() {  
    obstacles.clear();
    
    int obstacleWidth = 3 * TILE_SIZE;
    int obstacleHeight = 10 * TILE_SIZE;  // Adjust the height as needed

   

    obstacles.push_back({0, 350, obstacleHeight+70, obstacleWidth});//345
    obstacles.push_back({140, 0, obstacleWidth, obstacleHeight+200});//145
    obstacles.push_back({470, 110, obstacleHeight+70, obstacleWidth});//3rd
    obstacles.push_back({470, 180, obstacleWidth, obstacleHeight+200});//465

    }

    void Snake::handleInput(SDL_Event &e) {
     if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
           case SDLK_UP:
               if (direction != 1) direction = 0;
                break;
           case SDLK_DOWN:
                if (direction != 0) direction = 1;
                break;
           case SDLK_LEFT:
                if (direction != 3) direction = 2;
                break;
           case SDLK_RIGHT:
               if (direction != 2) direction = 3;
                break;
        }
     }
    }


    void Snake:: move(SDL_Renderer *renderer) {
    SDL_Rect newHead = body.front();
    switch (direction) {
        case 0: // Up
        if(newHead.y<=0)
            newHead.y=SCREEN_HEIGHT-TILE_SIZE;
        else
            newHead.y -= TILE_SIZE;
            break;
        case 1: // Down
        if(newHead.y>=SCREEN_HEIGHT-TILE_SIZE)
            newHead.y=0; 
        else
            newHead.y += TILE_SIZE;
            break;
        case 2: // Left
        if(newHead.x<=0)
            newHead.x=SCREEN_WIDTH-TILE_SIZE;
        else
            newHead.x -= TILE_SIZE;
            break;
        case 3: // Right
        if(newHead.x>=SCREEN_WIDTH-TILE_SIZE)
            newHead.x=0; 
        else
            newHead.x += TILE_SIZE;
            break;
    }
     body.insert(body.begin(), newHead);
    if(score != bonusOn && score%5==0){
        bonusfood();
        bonusCreateTime= SDL_GetTicks();
        bonusOn = score;
    } 

    p_time = SDL_GetTicks();

    if (p_time - bonusCreateTime >= 4000)
        handlebonus();

    if (newHead.x == food.x && newHead.y == food.y) {
        //grow(); 
        playeatsound();
        score++;
        
        spawnFood();
       
    } else {
        body.pop_back();
    } 
    //if (checkcollisionbonus()) {
       //spawnFood();
    //}
    if (checkcollisionbonus()) {
        score+=11;
        handlebonus();
    }
    if (checkCollision()) {
    SDL_RenderClear(renderer);
    renderGameOver(renderer); // Render "Game Over" screen
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
        std::cout << "Game Over!" << std::endl;
        SDL_Quit();
        exit(0);
    }

    }
    
    void Snake::playeatsound(){
      Mix_PlayChannel(-1,eatsound,0);
    }


    void Snake::render(SDL_Renderer *renderer) {

    SDL_Color textColor = {25, 0, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, ("Score: " + to_string(score)).c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {SCREEN_WIDTH/2-surface->w/2, 10, surface->w, surface->h}; // Adjust position as needed
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
 

    SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
    for (const auto &segment : body) {
        SDL_RenderFillRect(renderer, &segment);
    } 

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color to red
    SDL_RenderFillRect(renderer, &food); 

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Set color to yellow
    SDL_RenderFillRect(renderer, &bonus);

    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); 
    for (const auto &obstacle : obstacles) {
        SDL_RenderFillRect(renderer, &obstacle);
    }

   }


   bool Snake::checkCollision() {
    SDL_Rect head = body.front();

    for (auto it = body.begin() + 1; it != body.end(); ++it) {
        if (head.x == it->x && head.y == it->y) {
            return true;
        }
    }
   
    //if (head.x < 0 || head.x >= SCREEN_WIDTH || head.y < 0 || head.y >= SCREEN_HEIGHT) {
      //  return true;
    //} 
     
    for (const auto &obstacle : obstacles) {
        
        if (body.front().x >= obstacle.x &&
            body.front().x < obstacle.x + obstacle.w &&
            body.front().y >= obstacle.y &&
            body.front().y < obstacle.y + obstacle.h) 
                return true;  // Collision with obstacle
    } 

    return false;
   } 
    

    

    void Snake::grow() {
      if (!body.empty()) {
        // Get the last element in the body vector
        SDL_Rect tail = body.back();

        // Add a new rectangle adjacent to the tail (for example, to the left)
        body.push_back({tail.x - TILE_SIZE, tail.y, TILE_SIZE, TILE_SIZE});
      } else {
        // If the body is empty, just add a new rectangle at some default position
        body.push_back({320, 240, TILE_SIZE, TILE_SIZE});
      }
    }

    
    void Snake::spawnFood() {
        do{
    food.x = rand() % (SCREEN_WIDTH / TILE_SIZE) * TILE_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / TILE_SIZE) * TILE_SIZE;
        }while(isFoodCollidingWithObstacles());
    food.w = TILE_SIZE;
    food.h = TILE_SIZE;
    } 


    void Snake::bonusfood(){
        do{
            bonus.x=rand() % (SCREEN_WIDTH / TILE_SIZE) * TILE_SIZE;
            bonus.y=rand() % (SCREEN_HEIGHT / TILE_SIZE) * TILE_SIZE;
        }while(isFoodCollidingWithObstacles());
        bonus.w = TILE_SIZE*2;
        bonus.h = TILE_SIZE*2;
    }
    
    


    bool Snake::isFoodCollidingWithObstacles() {
    // Check collision with obstacles
      for (const auto &obstacle : obstacles) {
          if (food.x< obstacle.x + obstacle.w &&
            food.x + TILE_SIZE > obstacle.x &&
            food.y< obstacle.y + obstacle.h &&
            food.y + TILE_SIZE > obstacle.y) {
            return true;  // Collision with obstacle
          }
      }
       return false;  // No collision
    }

    bool Snake::checkcollisionbonus(){
        if (body.front().x <= bonus.x + bonus.w &&
        body.front().x + TILE_SIZE >= bonus.x &&
        body.front().y <= bonus.y + bonus.h &&
        body.front().y + TILE_SIZE >= bonus.y) {
        
        return true; 
        
    } 
    return false;
    }
    
    void Snake::renderGameOver(SDL_Renderer *renderer) {
    SDL_Color textColor = {255, 0, 0, 255}; // Red color
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Game Over", textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {SCREEN_WIDTH/2 - surface->w/2, SCREEN_HEIGHT/2 - surface->h/2, surface->w, surface->h}; // Center the text
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    }


    void Snake::handlebonus(){
        bonus={-1,-1,0,0};
    }

int main(int argc, char *argv[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    font = TTF_OpenFont("Sofia.otf", 24);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Snake snake;
    SDL_Event e;
    bool quit = false;
    
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            snake.handleInput(e);
        }

        snake.move(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        snake.render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }
    

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    }