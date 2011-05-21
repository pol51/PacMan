#include <Sprite.h>

#include <SDL_image.h>

Sprite::Sprite(const char *filename)
{
  _map = IMG_Load(filename);
  _dest.w = _dest.h = _src.w = _src.h = SPRITE_HEIGHT;
  _dest.x = _dest.y = _src.x = _src.y = 0;
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
  SDL_BlitSurface(_map, &_src, surface, &_dest);
}
