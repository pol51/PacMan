#include "Sprite.h"

const Sprite Sprite::null("");

Sprite::Sprite(const char* filename)
    : _map(SurfacesManager::loadImage(filename))
{
  if (_map)
    _imageCount = _map->h / SPRITE_HEIGHT;
  setDirection(eLeft);
}

Sprite::Sprite(const char* filename, const unsigned int x, const unsigned int y)
    : Sprite(filename)
{
  setPosition(x, y);
}

void Sprite::draw(SDL_Renderer* renderer)
{
  if (_pos.x < _dst.x)
    _pos.x += _src.w >> 3;
  if (_pos.x > _dst.x)
    _pos.x -= _src.w >> 3;
  if (_pos.y < _dst.y)
    _pos.y += _src.h >> 3;
  if (_pos.y > _dst.y)
    _pos.y -= _src.h >> 3;

  SDL_FRect srcFRect;
  SDL_FRect dstFRect;
  SDL_RectToFRect(&_src, &srcFRect);
  SDL_RectToFRect(&_pos, &dstFRect);

  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, _map.get());
  SDL_RenderTexture(renderer, tex, &srcFRect, &dstFRect);
  SDL_DestroyTexture(tex);
}
