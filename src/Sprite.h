#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL.h>

#include "Directions.h"
#include "Def.h"

class Sprite
{
  public:
    /*!
    * @brief ctors
    */
    Sprite(const Sprite &other);
    Sprite(const char* filename);
    Sprite(const char* filename, const unsigned int x, const unsigned int y);

    /*!
    * @brief dtor
    */
    virtual ~Sprite();

    /*!
    * @brief Draw the sprite to surface
    * @param surface the destination surface
    */
    void draw(SDL_Surface *surface);

    /*!
    * @brief Accessors
    */
    unsigned int width()  const { return _src.w; }
    unsigned int height() const { return _src.h; }
    unsigned int left()   const { return _pos.x; }
    unsigned int right()  const { return _pos.x + _src.w; }
    unsigned int top()    const { return _pos.y; }
    unsigned int bottom() const { return _pos.y + _src.h; }

    bool isInMove() const { return _pos.x != _dst.x || _pos.y != _dst.y; }

    void setDirection(const EDirection direction)
    {
      _direction = direction;
      _src.x = (unsigned int)_direction * _src.w;
    }

    void setPosition(const unsigned int x, const unsigned int y)
    {
      _pos.x = _dst.x = x;
      _pos.y = _dst.y = y;
    }

    void moveLeft(const unsigned int offset)
    {
      if (!isInMove())
      {
        setDirection(eLeft);
        _dst.x -= offset;
      }
    }

    void moveRight(const unsigned int offset)
    {
      if (!isInMove())
      {
        setDirection(eRight);
        _dst.x += offset;
      }
    }

    void moveUp(const unsigned int offset)
    {
      if (!isInMove())
      {
        setDirection(eUp);
        _dst.y -= offset;
      }
    }

    void moveDown(const unsigned int offset)
    {
      if (!isInMove())
      {
        setDirection(eDown);
        _dst.y += offset;
      }
    }

    void incStep()
    {
      _imageIndex = _imageIndex + _imageInc;
      if (_imageIndex == _imageCount - 1) _imageInc = -1;
      if (_imageIndex == 0) _imageInc = 1;
      _src.y = _imageIndex * SPRITE_HEIGHT;
    }

    const Sprite &operator=(const Sprite& other);

  protected:
    EDirection _direction{eLeft};
    SDL_Surface *_map{NULL};
    SDL_Rect _src{ 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_Rect _pos{ 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_Rect _dst{ 0, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
    unsigned short int _imageCount{1};
    unsigned short int _imageIndex{0};
    short _imageInc{1};

  public:
    static const Sprite null;
};

#endif
