#include "Game.h"

#include "Timer.h"

Game::Game(const char *filename)
{
  FILE *File(fopen(filename, "r"));
  if (!File)
  {
    fprintf(stderr, "Can't read level from %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char Line[1024], *C;
  unsigned int X{0}, Y{0};
  while (fgets(Line, 1024, File))
  {
    X = 0;
    for (C = Line; *C; ++C, ++X)
      addSprite(*C, X, Y);
    ++Y;
  }

  _width = X - (Y && *(C-1) == '\n'); // ignore new-line
  _heigth = Y;

  fclose(File);

  createScreen();
}

Game::~Game()
{
  SDL_FreeSurface(_screen);
  SDL_DestroyWindow(_window);
}

void Game::addSprite(const char type, const unsigned int x, const unsigned y)
{
  switch (type)
  {
    case '#':
      _walls.push(Sprite("res/wall.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
      break;
    case '.':
      _smallPills.push(Sprite("res/pill_small.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
      break;
    case '*':
      _bigPills.push(Sprite("res/pill_big.png", x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
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
  _window = SDL_CreateWindow("PacMan",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    _width * SPRITE_WIDTH, _heigth * SPRITE_HEIGHT,
    SDL_WINDOW_SHOWN);
  _screen = SDL_GetWindowSurface(_window);

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

void Game::draw()
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);

  for (int i = _walls.size(); --i >= 0; )
    _walls[i].draw(_renderer);
  for (int i = _smallPills.size(); --i >= 0; )
    _smallPills[i].draw(_renderer);
  for (int i = _bigPills.size(); --i >= 0; )
    _bigPills[i].draw(_renderer);

  _blueGhost.draw(_renderer);
  _orangeGhost.draw(_renderer);
  _pinkGhost.draw(_renderer);
  _redGhost.draw(_renderer);

  _pacman.draw(_renderer);
  _pacman.incStep();

  SDL_RenderPresent(_renderer);
}

void Game::handleKeys()
{
  SDL_Event Event;
  while (SDL_PollEvent(&Event))
  {
    switch (Event.type)
    {
      case SDL_KEYDOWN:
        if ((unsigned int)(Event.key.keysym.sym - 273) < 5)
          _keyDown++;
        _lastEvent = Event;
        break;
      case SDL_KEYUP:
        if ((unsigned int)(Event.key.keysym.sym - 273) < 5)
          _keyDown--;
        if (!_keyDown)
          _lastEvent = SDL_Event();
        break;
      case SDL_QUIT:
        _running = false;
        return;
    }
  }

  switch (_lastEvent.key.keysym.sym)
  {
    case SDLK_ESCAPE:  // exit
      _running = false;
      break;
    case SDLK_UP: // up
      _pacman.moveUp(_pacman.top() > 0 ? SPRITE_HEIGHT : 0);
      break;
    case SDLK_DOWN: // down
      _pacman.moveDown(_pacman.bottom() < _heigth * SPRITE_HEIGHT ? SPRITE_HEIGHT : 0);
      break;
    case SDLK_RIGHT: // right
      _pacman.moveRight(_pacman.right() < _width * SPRITE_WIDTH ? SPRITE_WIDTH : 0);
      break;
    case SDLK_LEFT: // left
      _pacman.moveLeft(_pacman.left() > 0 ? SPRITE_WIDTH : 0);
      break;
  }
}

void Game::run()
{
  Timer T(30);

  _running = true;
  while (_running)
  {
    T.Start();

    handleKeys();
    draw();

    while (!T.IsFPSReached());
  }
}
