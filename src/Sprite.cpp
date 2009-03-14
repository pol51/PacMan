#include <Sprite.h>

#include <SDL_image.h>

Sprite::Sprite(const char *filename)
{
  _map = IMG_Load(filename);
  /*SDL_Surface *image = IMG_Load(filename);
  _map = SDL_DisplayFormat(image);
  SDL_FreeSurface(image);
  unsigned int colorkey = SDL_MapRGB(_map->format, 0x00, 0x00, 0x00);
  SDL_SetColorKey(_map, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);*/
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
