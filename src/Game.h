#ifndef __GAME_H__
#define __GAME_H__

#include "Sprite.h"
#include "XVector.h"

class Game
{
  public:
    Game(const char *filename);
    ~Game();

    void draw();

    void run();

    SDL_Surface *screen() {return _screen;}
    const unsigned int width() const { return _width; }
    const unsigned int heigth() const { return _heigth; }

  protected:
    void addSprite(const char type, const unsigned int x, const unsigned y);
    void createScreen();

  protected:
    unsigned int _width{0};
    unsigned int _heigth{0};

    SDL_Surface *_screen{NULL};

    XVector<Sprite> _walls;
    Sprite  _pacman{Sprite::null};
    Sprite  _blueGhost{Sprite::null};
    Sprite  _orangeGhost{Sprite::null};
    Sprite  _pinkGhost{Sprite::null};
    Sprite  _redGhost{Sprite::null};
};

#endif
