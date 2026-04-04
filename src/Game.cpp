#include "Game.h"

#include <stdio.h>

#include "Timer.h"

Game::Game(const char* filename)
{
  FILE* file(fopen(filename, "r"));
  if (!file)
  {
    SDL_Log("Can't read level from %s", filename);
    exit(EXIT_FAILURE);
  }

  char line[1024], *c;
  unsigned int x {0}, y {0};
  while (fgets(line, 1024, file))
  {
    x = 0;
    for (c = line; *c; ++c, ++x)
      addSprite(*c, x, y);
    ++y;
  }

  _width  = x - (y && *(c - 1) == '\n'); // ignore new-line
  _heigth = y;

  fclose(file);

  createScreen();
}

Game::~Game()
{
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
}

void Game::addSprite(const char type, const unsigned int x, const unsigned y)
{
  switch (type)
  {
    case '#':
      _walls.push(Sprite("res/wall.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
      break;
    case '.':
      _smallPills.push(Sprite("res/pill_small.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
      break;
    case '*':
      _bigPills.push(Sprite("res/pill_big.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT));
      break;
    case 'A':
      _pacman = Sprite("res/pacman.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
      break;
    case 'B':
      _blueGhost = Sprite("res/ghost_blue.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
      break;
    case 'O':
      _orangeGhost = Sprite("res/ghost_orange.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
      break;
    case 'P':
      _pinkGhost = Sprite("res/ghost_pink.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
      break;
    case 'R':
      _redGhost = Sprite("res/ghost_red.png", x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
      break;
    default:
      return;
  }
}

void Game::createScreen()
{
  _window   = SDL_CreateWindow("PacMan", _width * SPRITE_WIDTH, _heigth * SPRITE_HEIGHT, 0);
  _renderer = SDL_CreateRenderer(_window, nullptr);
}

void Game::draw()
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);

  for (int i = _walls.size(); --i >= 0;)
    _walls[i].draw(_renderer);
  for (int i = _smallPills.size(); --i >= 0;)
    _smallPills[i].draw(_renderer);
  for (int i = _bigPills.size(); --i >= 0;)
    _bigPills[i].draw(_renderer);

  _blueGhost.draw(_renderer);
  _orangeGhost.draw(_renderer);
  _pinkGhost.draw(_renderer);
  _redGhost.draw(_renderer);

  _pacman.draw(_renderer);
  _pacman.incStep();

  SDL_RenderPresent(_renderer);
}

void Game::eatPills()
{
  if (_pacman.isInMove())
    return;

  int px = _pacman.left();
  int py = _pacman.top();

  for (int i = _smallPills.size(); --i >= 0;)
    if (_smallPills[i].left() == (unsigned)px && _smallPills[i].top() == (unsigned)py)
    {
      _smallPills.remove(i);
      break;
    }

  for (int i = _bigPills.size(); --i >= 0;)
    if (_bigPills[i].left() == (unsigned)px && _bigPills[i].top() == (unsigned)py)
    {
      _bigPills.remove(i);
      break;
    }
}

bool Game::hasWallAt(int x, int y) const
{
  for (int i = _walls.size(); --i >= 0;)
    if (_walls[i].left() == (unsigned)x && _walls[i].top() == (unsigned)y)
      return true;
  return false;
}

void Game::handleKeys()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_EVENT_KEY_DOWN:
        if ((unsigned int)(event.key.key - 273) < 5)
          _keyDown++;
        _lastEvent = event;
        break;
      case SDL_EVENT_KEY_UP:
        if ((unsigned int)(event.key.key - 273) < 5)
          _keyDown--;
        if (!_keyDown)
          _lastEvent = SDL_Event();
        break;
      case SDL_EVENT_QUIT:
        _running = false;
        return;
    }
  }

  int px = _pacman.left();
  int py = _pacman.top();

  switch (_lastEvent.key.key)
  {
    case SDLK_ESCAPE:
      _running = false;
      break;
    case SDLK_UP:
      if (py > 0 && !hasWallAt(px, py - SPRITE_HEIGHT))
        _pacman.moveUp(SPRITE_HEIGHT);
      break;
    case SDLK_DOWN:
      if (_pacman.bottom() < _heigth * SPRITE_HEIGHT && !hasWallAt(px, py + SPRITE_HEIGHT))
        _pacman.moveDown(SPRITE_HEIGHT);
      break;
    case SDLK_RIGHT:
      if (_pacman.right() < _width * SPRITE_WIDTH && !hasWallAt(px + SPRITE_WIDTH, py))
        _pacman.moveRight(SPRITE_WIDTH);
      break;
    case SDLK_LEFT:
      if (px > 0 && !hasWallAt(px - SPRITE_WIDTH, py))
        _pacman.moveLeft(SPRITE_WIDTH);
      break;
  }
}

void Game::run()
{
  Timer timer(30);

  _running = true;
  while (_running)
  {
    timer.Start();

    eatPills();
    handleKeys();
    draw();

    while (!timer.IsFPSReached())
      ;
  }
}
