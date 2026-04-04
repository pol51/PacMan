#ifndef __GAME_H__
#define __GAME_H__

#include "Ghost.h"
#include "Sprite.h"
#include "XVector.h"

class Game
{
public:
  Game(const char* filename);
  ~Game();

  void run();

  unsigned int width() const { return _width; }
  unsigned int heigth() const { return _heigth; }

protected:
  void addSprite(const char type, const unsigned int x, const unsigned y);
  void createScreen();
  void draw();
  void handleKeys();
  void movePacman();
  void eatPills();
  void checkGhosts();
  void moveGhosts();
  void updateGhostMode();
  bool hasWallAt(int x, int y) const;
  static bool hasWallAtStatic(int x, int y);

protected:
  bool _running {false};

  unsigned int _width {0};
  unsigned int _heigth {0};

  EDirection _nextDirection {eLeft};

  SDL_Window* _window {NULL};
  SDL_Renderer* _renderer {NULL};

  XVector<Sprite> _walls;
  XVector<Sprite> _smallPills;
  XVector<Sprite> _bigPills;
  int _powerUpFrames {0};
  int _modeFrames {0};
  int _modeIndex {0};

  Sprite _pacman = Sprite::null;
  Ghost* _ghosts[4] {nullptr, nullptr, nullptr, nullptr};

  static Game* _instance;
};

#endif
