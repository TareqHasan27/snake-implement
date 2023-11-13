#include<iostream>
#include<SDL2/SDL.h>
#include<vector>
#include<algorithm>
#include<deque>
int main(int argc, char* args[])
{
SDL_Init(SDL_INIT_EVERYTHING);
 SDL_Window* window = nullptr;
 SDL_Renderer* renderer = nullptr;
 SDL_CreateWindowAndRenderer(640,480,0,&window,&renderer);
   SDL_Event e;
   enum Direction{
     DOWN,
     LEFT,
     RIGHT,
     UP
   };
  bool running =true;
  int dir=0;
  //Head position
  //
  SDL_Rect head {320,240,5,5};
  
  while(running){
  //Check input
  //
  while(SDL_PollEvent(&e)){
    if(e.type==SDL_QUIT) running = false;
    if(e.type==SDL_KEYDOWN){
                                                        // if(e.key.keysym.sym==SDLK_DOWN){dir=DOWN;}
                                                         //  if(e.key.keysym.sym==SDLK_UP){dir=UP;}
                                                         //  if(e.key.keysym.sym==SDLK_LEFT){dir=LEFT;}
                                                        //  if(e.key.keysym.sym==SDLK_RIGHT){dir=RIGHT;}
        //Move
        //
        if(SDLK_RIGHT==e.key.keysym.sym){
           head.x +=5;
           break;
        }
            else if(SDLK_LEFT==e.key.keysym.sym){
           head.x-=5;
           break;
            }
            else if(SDLK_UP==e.key.keysym.sym){
            head.y-=5;
            break;
            }
            else if(SDLK_DOWN==e.key.keysym.sym){
            head.y+=5;
            break;
            }
    }
  }
  //Clear window
  //
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_RenderClear(renderer);

  //Draw body
  //
  SDL_SetRenderDrawColor(renderer,255,255,255,255);
  SDL_RenderFillRect(renderer,&head);

  SDL_RenderPresent(renderer);
  SDL_Delay(25);

  }

}