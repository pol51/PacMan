#include "SurfacesManager.h"

#include <SDL3_image/SDL_image.h>

XVector<SurfacesManager::SSurface> SurfacesManager::_surfaces;

SDL_Surface* SurfacesManager::loadImage(const char* filename)
{
  // lookup for existing surface
  for (int i = _surfaces.size(); --i >= 0;)
    if (!strcmp(filename, _surfaces[i].filename))
    {
      ++_surfaces[i].count;
      return _surfaces[i].surface;
    }

  // load the sdl surface
  SDL_Surface* Map {NULL};
  if (strlen(filename))
  {
    Map = IMG_Load(filename);
    if (!Map)
    {
      SDL_Log("Error loading %s", filename);
      return NULL;
    }
    SDL_Log("SurfacesManager::loadSurface %s", filename);
  }
  else
    Map = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGB48);

  // store the surface
  _surfaces.push(SSurface());
  SSurface& NewSurface = _surfaces[_surfaces.size() - 1];
  NewSurface.count     = 1;
  const int FilenameLen(strlen(filename));
  NewSurface.filename = (char*)malloc(FilenameLen * sizeof(char));
  strcpy(NewSurface.filename, filename);
  NewSurface.surface = Map;

  return Map;
}

SDL_Surface* SurfacesManager::getSurface(const SDL_Surface* surface)
{
  // lookup for existing surface
  for (int i = _surfaces.size(); --i >= 0;)
    if (surface == _surfaces[i].surface)
    {
      ++_surfaces[i].count;
      return _surfaces[i].surface;
    }
  return NULL;
}

void SurfacesManager::freeSurface(const SDL_Surface* surface)
{
  // retrieve the surface
  for (int i = _surfaces.size(); --i >= 0;)
    if (surface == _surfaces[i].surface)
      if (!--_surfaces[i].count)
      {
        SDL_Log("SurfacesManager::freeSurface %s", _surfaces[i].filename);
        SDL_DestroySurface(_surfaces[i].surface);
        free(_surfaces[i].filename);
        _surfaces.remove(i);
        return;
      }
}
