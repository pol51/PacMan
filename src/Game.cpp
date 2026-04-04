#include "Game.h"

#include <stdio.h>

#include "Timer.h"

Game* Game::_instance = nullptr;

// chase/scatter alternation durations in frames (at 30 fps)
// scatter 7s, chase 20s, scatter 7s, chase 20s, scatter 5s, chase 20s, scatter 5s, chase forever
static const int MODE_DURATIONS[] = {
  7 * 30, 20 * 30, 7 * 30, 20 * 30, 5 * 30, 20 * 30, 5 * 30, 0
};
static const int MODE_COUNT = sizeof(MODE_DURATIONS) / sizeof(MODE_DURATIONS[0]);

Game::Game(const char* filename)
{
  _instance = this;

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
  for (auto& ghost : _ghosts)
    delete ghost;
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  _instance = nullptr;
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
    case 'R':
      _ghosts[Ghost::eRed] = new Ghost("res/ghost_red.png",
        x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Ghost::eRed, 0, 0);
      break;
    case 'P':
      _ghosts[Ghost::ePink] = new Ghost("res/ghost_pink.png",
        x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Ghost::ePink, 0, 0);
      break;
    case 'B':
      _ghosts[Ghost::eBlue] = new Ghost("res/ghost_blue.png",
        x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Ghost::eBlue, 0, 0);
      break;
    case 'O':
      _ghosts[Ghost::eOrange] = new Ghost("res/ghost_orange.png",
        x * SPRITE_WIDTH, y * SPRITE_HEIGHT, Ghost::eOrange, 0, 0);
      break;
    default:
      return;
  }
}

void Game::createScreen()
{
  _window   = SDL_CreateWindow("PacMan", _width * SPRITE_WIDTH, _heigth * SPRITE_HEIGHT, 0);
  _renderer = SDL_CreateRenderer(_window, nullptr);

  // set scatter corners now that map dimensions are known
  int w = (int)_width - 1;
  int h = (int)_heigth - 1;
  if (_ghosts[Ghost::eRed])    _ghosts[Ghost::eRed]->setScatterTarget(w, 0);
  if (_ghosts[Ghost::ePink])   _ghosts[Ghost::ePink]->setScatterTarget(0, 0);
  if (_ghosts[Ghost::eBlue])   _ghosts[Ghost::eBlue]->setScatterTarget(w, h);
  if (_ghosts[Ghost::eOrange]) _ghosts[Ghost::eOrange]->setScatterTarget(0, h);
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

  for (auto ghost : _ghosts)
    if (ghost)
      ghost->draw(_renderer);

  _pacman.draw(_renderer);
  _pacman.incStep();

  SDL_RenderPresent(_renderer);
}

void Game::eatPills()
{
  for (int i = _smallPills.size(); --i >= 0;)
    if (_pacman.overlaps(_smallPills[i]))
    {
      _smallPills.remove(i);
      break;
    }

  for (int i = _bigPills.size(); --i >= 0;)
    if (_pacman.overlaps(_bigPills[i]))
    {
      _bigPills.remove(i);
      _powerUpFrames = 30 * 10; // 10 seconds at 30 fps
      for (auto ghost : _ghosts)
        if (ghost && ghost->isVisible())
          ghost->setMode(Ghost::eFrightened);
      break;
    }
}

void Game::checkGhosts()
{
  if (_powerUpFrames > 0)
  {
    --_powerUpFrames;
    if (_powerUpFrames == 0)
    {
      // frightened mode ends, restore previous mode
      Ghost::Mode mode = (_modeIndex % 2 == 0) ? Ghost::eScatter : Ghost::eChase;
      for (auto ghost : _ghosts)
        if (ghost && ghost->isVisible())
          ghost->setMode(mode);
    }
  }

  for (auto ghost : _ghosts)
  {
    if (!ghost || !ghost->isVisible())
      continue;
    if (!_pacman.overlaps(*ghost))
      continue;

    if (_powerUpFrames > 0)
      ghost->setVisible(false);
    else
    {
      _pacman.setVisible(false);
      _running = false;
    }
  }
}

bool Game::hasWallAt(int x, int y) const
{
  for (int i = _walls.size(); --i >= 0;)
    if (_walls[i].left() == (unsigned)x && _walls[i].top() == (unsigned)y)
      return true;
  return false;
}

bool Game::hasWallAtStatic(int x, int y)
{
  return _instance->hasWallAt(x, y);
}

void Game::moveGhosts()
{
  Ghost* blinky = _ghosts[Ghost::eRed];
  if (!blinky)
    return;

  for (auto ghost : _ghosts)
    if (ghost && ghost->isVisible())
      ghost->update(_pacman, *blinky, hasWallAtStatic, _width, _heigth);
}

void Game::updateGhostMode()
{
  if (_powerUpFrames > 0)
    return; // don't advance mode timer during power-up

  if (_modeIndex >= MODE_COUNT - 1)
    return; // permanent chase

  ++_modeFrames;
  if (_modeFrames >= MODE_DURATIONS[_modeIndex])
  {
    _modeFrames = 0;
    ++_modeIndex;
    Ghost::Mode mode = (_modeIndex % 2 == 0) ? Ghost::eScatter : Ghost::eChase;
    for (auto ghost : _ghosts)
      if (ghost && ghost->isVisible())
        ghost->setMode(mode);
  }
}

void Game::handleKeys()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_EVENT_KEY_DOWN:
        switch (event.key.key)
        {
          case SDLK_ESCAPE: _running = false; return;
          case SDLK_UP:     _nextDirection = eUp;    break;
          case SDLK_DOWN:   _nextDirection = eDown;  break;
          case SDLK_RIGHT:  _nextDirection = eRight; break;
          case SDLK_LEFT:   _nextDirection = eLeft;  break;
        }
        break;
      case SDL_EVENT_QUIT:
        _running = false;
        return;
    }
  }
}

void Game::movePacman()
{
  if (_pacman.isInMove())
    return;

  int px = _pacman.left();
  int py = _pacman.top();

  // try the desired direction first
  auto canMove = [&](EDirection dir) -> bool {
    switch (dir)
    {
      case eUp:    return py > 0 && !hasWallAt(px, py - SPRITE_HEIGHT);
      case eDown:  return _pacman.bottom() < _heigth * SPRITE_HEIGHT && !hasWallAt(px, py + SPRITE_HEIGHT);
      case eRight: return _pacman.right() < _width * SPRITE_WIDTH && !hasWallAt(px + SPRITE_WIDTH, py);
      case eLeft:  return px > 0 && !hasWallAt(px - SPRITE_WIDTH, py);
    }
    return false;
  };

  auto doMove = [&](EDirection dir) {
    switch (dir)
    {
      case eUp:    _pacman.moveUp(SPRITE_HEIGHT);    break;
      case eDown:  _pacman.moveDown(SPRITE_HEIGHT);  break;
      case eRight: _pacman.moveRight(SPRITE_WIDTH);  break;
      case eLeft:  _pacman.moveLeft(SPRITE_WIDTH);   break;
    }
  };

  if (canMove(_nextDirection))
    doMove(_nextDirection);
  else if (canMove(_pacman.direction()))
    doMove(_pacman.direction());
}

void Game::run()
{
  Timer timer(30);

  _running = true;
  while (_running)
  {
    timer.Start();

    eatPills();
    checkGhosts();
    updateGhostMode();
    moveGhosts();
    handleKeys();
    movePacman();
    draw();

    while (!timer.IsFPSReached())
      ;
  }
}
