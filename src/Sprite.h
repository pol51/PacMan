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
    SDL_Rect _pos;
    SDL_Rect _dst;
    unsigned short int _imageCount;
    unsigned short int _imageIndex;
    short _imageInc;

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
    unsigned int Width()  const { return _src.w; }
    unsigned int Height() const { return _src.h; }
    unsigned int Left()   const { return _pos.x; }
    unsigned int Right()  const { return _pos.x + _src.w; }
    unsigned int Top()    const { return _pos.y; }
    unsigned int Bottom() const { return _pos.y + _src.h; }

    bool InMove() const { return _pos.x != _dst.x || _pos.y != _dst.y; }

    void Direction(const EDirection direction)
    {
        _direction = direction;
        _src.x = (unsigned int)_direction * _src.w;
    }

    void Position(const unsigned int x, const unsigned int y)
    {
        _pos.x = _dst.x = x;
        _pos.y = _dst.y = y;
    }

    void MoveLeft(const unsigned int offset)
    {
        if (!InMove())
        {
            Direction(eLeft);
            _dst.x -= offset;
        }
    }

    void MoveRight(const unsigned int offset)
    {
        if (!InMove())
        {
            Direction(eRight);
            _dst.x += offset;
        }
    }

    void MoveUp(const unsigned int offset)
    {
        if (!InMove())
        {
            Direction(eUp);
            _dst.y -= offset;
        }
    }

    void MoveDown(const unsigned int offset)
    {
        if (!InMove())
        {
            Direction(eDown);
            _dst.y += offset;
        }
    }

    void IncStep()
    {
        _imageIndex = _imageIndex + _imageInc;
        if (_imageIndex == _imageCount - 1) _imageInc = -1;
        if (_imageIndex == 0) _imageInc = 1;
        _src.y = _imageIndex * SPRITE_HEIGHT;
    }
};

#endif
