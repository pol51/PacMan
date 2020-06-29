#ifndef __GAME_H__
#define __GAME_H__

#include "Sprite.h"
#include "XVector.h"

class Game
{
  public:
    Game(const char *filename);
    ~Game();

    void run();

    SDL_Surface *screen() {return _screen;}
    const unsigned int width() const { return _width; }
    const unsigned int heigth() const { return _heigth; }

  protected:
    void addSprite(const char type, const unsigned int x, const unsigned y);
    void createScreen();
    void draw();
    void handleKeys();

  protected:
    bool _running{false};

    unsigned int _width{0};
    unsigned int _heigth{0};

    int _keyDown{0};
    SDL_Event _lastEvent;

    SDL_Window *_window{NULL};
    SDL_Surface *_screen{NULL};

    SDL_Renderer *_renderer{NULL};

    XVector<Sprite> _walls;
    XVector<Sprite> _smallPills;
    XVector<Sprite> _bigPills;
    Sprite  _pacman         = Sprite::null;
    Sprite  _blueGhost      = Sprite::null;
    Sprite  _orangeGhost    = Sprite::null;
    Sprite  _pinkGhost      = Sprite::null;
    Sprite  _redGhost       = Sprite::null;
};

#endif
