#include <Sprite.h>

#include <SDL_image.h>

#include <cstdio>

Sprite::Sprite(const char *filename)
{
  _map = IMG_Load(filename);
  if (!_map)
    fprintf(stderr, "Error loading %s\n", filename);
  _pos.w = _pos.h = _src.w = _src.h = SPRITE_HEIGHT;
  _pos.x = _pos.y = _src.x = _src.y = _dst.x = _dst.y = 0;
  _imageCount = _map->h / SPRITE_HEIGHT;
  _imageIndex = 0;
  _imageInc = 1;
  Direction(eLeft);
}

Sprite::~Sprite()
{
  SDL_FreeSurface(_map);
}

void Sprite::Draw(SDL_Surface *surface)
{
  if (_pos.x < _dst.x) _pos.x += _src.w>>3;
  if (_pos.x > _dst.x) _pos.x -= _src.w>>3;
  if (_pos.y < _dst.y) _pos.y += _src.h>>3;
  if (_pos.y > _dst.y) _pos.y -= _src.h>>3;

  SDL_BlitSurface(_map, &_src, surface, &_pos);
}
