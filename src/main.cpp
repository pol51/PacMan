#include <Def.h>
#include <Sprite.h>
#include <Timer.h>

// SDL
#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Surface *screen = SDL_SetVideoMode(
    GAME_WIDTH, GAME_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  
  SDL_Surface *background = IMG_Load("../res/background.png");

  Sprite PacMan("../res/pacman.png");
  Sprite Ghost("../res/ghost_blue.png");

  int keyDown = 0;

  SDL_Event event, lastEvent, emptyEvent;
  Timer T(25);
  bool inGame = true;
  while(inGame)
  {
    //update timer
    T.Start();

    //events
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
          if ((unsigned int)(event.key.keysym.sym - 273) < 5)
            keyDown++;
          lastEvent = event;
          break;
        case SDL_KEYUP:
          if ((unsigned int)(event.key.keysym.sym - 273) < 5)
            keyDown--;
          if (keyDown == 0)
            lastEvent = emptyEvent;
          break;
      }
    }
    
    switch(lastEvent.key.keysym.sym)
    {
      case 27:  // exit
        inGame = false;
        break;
      case 273: // up
        PacMan.MoveUp(PacMan.Top()>0?PacMan.Height()>>2:0);
        break;
      case 274: // down
        PacMan.MoveDown(PacMan.Bottom()<GAME_HEIGHT?PacMan.Height()>>2:0);
        break;
      case 275: // right
        PacMan.MoveRight(PacMan.Right()<GAME_WIDTH?PacMan.Width()>>2:0);
        break;
      case 276: // left
        PacMan.MoveLeft(PacMan.Left()>0?PacMan.Width()>>2:0);
        break;
    }

    //draw
    SDL_BlitSurface(background, NULL, screen, NULL);
    Ghost.Draw(screen);
    PacMan.Draw(screen);
    PacMan.IncStep();
    SDL_Flip(screen);

    while (!T.IsFPSReached());
  }

  SDL_FreeSurface(screen);
  SDL_FreeSurface(background);
  
  return EXIT_SUCCESS;
}
