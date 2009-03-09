#ifndef __SPRITE_H__
#define __SPRITE_H_

#include <SDL.h>

#include <Def.h>

class Sprite
{
  protected:
    EDirection _direction;
    SDL_Surface *_map;
    SDL_Rect _src;
    SDL_Rect _dest;

  public:
    /*!
    * @brief ctor
    */
    Sprite(const char* filename);

    /*!
    * @brief dtor
    */
    virtual ~Sprite();

    /*!
    * @brief Draw the sprite to surface
    * @param surface the destination surface
    */
    void Draw(SDL_Surface *surface);

    /*!
    * @brief Accessors
    */
    unsigned int Width() const  { return _src.w; }
    unsigned int Height() const { return _src.h; }
    unsigned int Left() const   { return _dest.x; }
    unsigned int Right() const  { return _dest.x + _src.w; }
    unsigned int Top() const    { return _dest.y; }
    unsigned int Bottom() const { return _dest.y + _src.h; }

    void Direction(const EDirection direction)
    {
      _direction = direction;
      _src.x = (unsigned int)_direction * _src.w;
    }

    void Position(const unsigned int x, const unsigned int y)
    {
      _dest.x = x;
      _dest.y = y;
    }

    void MoveLeft(const unsigned int offset)
    {
      Direction(eLeft);
      _dest.x -= offset;
    }

    void MoveRight(const unsigned int offset)
    {
      Direction(eRight);
      _dest.x += offset;
    }

    void MoveUp(const unsigned int offset)
    {
      Direction(eUp);
      _dest.y -= offset;
    }

    void MoveDown(const unsigned int offset)
    {
      Direction(eDown);
      _dest.y += offset;
    }

};

#endif
