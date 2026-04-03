#include "Game.h"

int main(int, char**)
{
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0)
  {
    SDL_Log("SDL_Init() Error: %s", SDL_GetError());
    return -1;
  }

  Game("data/default_level.txt").run();

  return EXIT_SUCCESS;
}
