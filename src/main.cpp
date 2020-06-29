#include "Game.h"

int main(int, char **)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  Game("data/default_level.txt").run();

  return EXIT_SUCCESS;
}
