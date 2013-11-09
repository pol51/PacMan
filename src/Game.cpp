#include "Game.h"

#include "Def.h"
#include "Timer.h"

#include <cstdio>
#include <cstdlib>

Game::Game(const char *filename)
{
  FILE *File(fopen(filename, "r"));
  if (!File)
  {
    fprintf(stderr, "Can't read level from %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char Line[1024];
  unsigned int X{0}, Y{0};
  while (fgets(Line, 1024, File))
  {
    X = 0;
    for (char *C{Line}; *C; ++C, ++X)
      addSprite(*C, X, Y);
    ++Y;
  }

  _width = X;
  _heigth = Y;

  fclose(File);

  createScreen();
}

Game::~Game()
{
  SDL_FreeSurface(_screen);
}

void Game::addSprite(const char type, const unsigned int x, const unsigned y)
{
  switch (type)
  {
    case '#':
      _walls.push(Sprite("res/wall.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
      break;
    case 'A':
      _pacman = Sprite("res/pacman.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
      break;
    case 'B':
      _blueGhost = Sprite("res/ghost_blue.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
      break;
    case 'O':
      _orangeGhost = Sprite("res/ghost_orange.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
      break;
    case 'P':
      _pinkGhost = Sprite("res/ghost_pink.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
      break;
    case 'R':
      _redGhost = Sprite("res/ghost_red.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
      break;
    default:
      return;
  }
}

void Game::createScreen()
{
  _screen = SDL_SetVideoMode(_width * SPRITE_WIDTH, _heigth * SPRITE_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (!_screen)
  {
    fprintf(stderr, "Can't initialize screen in 32 bits, trying 16\n");
    _screen = SDL_SetVideoMode(_width * SPRITE_WIDTH, _heigth * SPRITE_HEIGHT, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!_screen)
    {
      fprintf(stderr, "Can't initialize screen in 16 bits, exiting\n");
      exit(EXIT_FAILURE);
    }
  }
}

void Game::draw()
{
  for (int i = _walls.size(); --i >= 0; )
    _walls[i].draw(_screen);

  _pacman.draw(_screen);
  _pacman.incStep();

  _blueGhost.draw(_screen);
  _orangeGhost.draw(_screen);
  _pinkGhost.draw(_screen);
  _redGhost.draw(_screen);
}

void Game::run()
{
  int KeyDown = 0;
  SDL_Event Event, LastEvent;
  Timer T(30);
  bool InGame = true;

  while (InGame)
  {
    //update timer
    T.Start();

    //events
    while (SDL_PollEvent(&Event))
    {
      switch (Event.type)
      {
        case SDL_KEYDOWN:
          if ((unsigned int)(Event.key.keysym.sym - 273) < 5)
            KeyDown++;
          LastEvent = Event;
          break;
        case SDL_KEYUP:
          if ((unsigned int)(Event.key.keysym.sym - 273) < 5)
            KeyDown--;
          if (KeyDown == 0)
            LastEvent = SDL_Event();
          break;
      }
    }

    switch (LastEvent.key.keysym.sym)
    {
      case 27:  // exit
        InGame = false;
        break;
      case 273: // up
        _pacman.moveUp(_pacman.top() > 0 ? SPRITE_HEIGHT : 0);
        break;
      case 274: // down
        _pacman.moveDown(_pacman.bottom() < _heigth * SPRITE_HEIGHT ? SPRITE_HEIGHT : 0);
        break;
      case 275: // right
        _pacman.moveRight(_pacman.right() < _width * SPRITE_WIDTH ? SPRITE_WIDTH : 0);
        break;
      case 276: // left
        _pacman.moveLeft(_pacman.left() > 0 ? SPRITE_WIDTH : 0);
        break;
    }

    //draw
    SDL_FillRect(_screen, NULL, 0);

    draw();

    SDL_Flip(_screen);

    while (!T.IsFPSReached());
  }
}
