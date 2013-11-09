#include "Sprite.h"

#include "SurfacesManager.h"

const Sprite Sprite::null("");

Sprite::Sprite(const Sprite &other)
{
  *this = other;
}

Sprite::Sprite(const char *filename)
{
  _map = SurfacesManager::loadImage(filename);
  if (_map)
    _imageCount = _map->h / SPRITE_HEIGHT;
  setDirection(eLeft);
}

Sprite::Sprite(const char *filename, const unsigned int x, const unsigned int y) : Sprite(filename)
{
  setPosition(x, y);
}

Sprite::~Sprite()
{
  SurfacesManager::freeSurface(_map);
}

void Sprite::draw(SDL_Surface *surface)
{
  if (_pos.x < _dst.x) _pos.x += _src.w>>3;
  if (_pos.x > _dst.x) _pos.x -= _src.w>>3;
  if (_pos.y < _dst.y) _pos.y += _src.h>>3;
  if (_pos.y > _dst.y) _pos.y -= _src.h>>3;

  SDL_BlitSurface(_map, &_src, surface, &_pos);
}

const Sprite &Sprite::operator=(const Sprite& other)
{
  _direction = other._direction;
  _map = SurfacesManager::getSurface(other._map);
  _src = other._src;
  _pos = other._pos;
  _dst = other._dst;
  _imageCount = other._imageCount;
  _imageIndex = other._imageIndex;
  _imageInc   = other._imageInc;
}
