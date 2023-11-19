#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 20;

class Snake {
public:
    Snake();
    void handleInput(SDL_Event &e);
    void move();
    void render(SDL_Renderer *renderer);
    int score=0;

private:
    SDL_Rect food;
    vector<SDL_Rect> body;
    int direction; // 0: up, 1: down, 2: left, 3: right 
    
};

Snake::Snake() {
    SDL_Rect head = {0, 0, TILE_SIZE, TILE_SIZE};
    body.push_back(head);
    direction = 3; // Start moving to the right
   
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

void Snake:: move() {
    SDL_Rect newHead = body.front();
    switch (direction) {
        case 0: // Up
            newHead.y -= TILE_SIZE;
            break;
        case 1: // Down
            newHead.y += TILE_SIZE;
            break;
        case 2: // Left
            newHead.x -= TILE_SIZE;
            break;
        case 3: // Right
            newHead.x += TILE_SIZE;
            break;
    }

    body.insert(body.begin(), newHead);
}

void Snake::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto &segment : body) {
        SDL_RenderFillRect(renderer, &segment);
    }
}

#undef main // Undefine main to prevent conflict
int WinMain(int argc, char* argv[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Init(SDL_INIT_VIDEO);

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

        snake.move();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        snake.render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
