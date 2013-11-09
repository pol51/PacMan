#include "SurfacesManager.h"

#include <SDL_image.h>

XVector<SurfacesManager::SSurface> SurfacesManager::_surfaces;

SDL_Surface *SurfacesManager::loadImage(const char *filename)
{
  // lookup for existing surface
  for (int i = _surfaces.size(); --i >= 0; )
    if (!strcmp(filename, _surfaces[i].filename))
    {
      ++_surfaces[i].count;
      return _surfaces[i].surface;
    }

  // load the sdl surface
  SDL_Surface *Map{NULL};
  if (strlen(filename))
  {
    Map = IMG_Load(filename);
    if (!Map)
    {
      fprintf(stderr, "Error loading %s\n", filename);
      return NULL;
    }
    fprintf(stderr, "SurfacesManager::loadSurface %s\n", filename);
  }
  else
    Map = SDL_CreateRGBSurface(0, 0, 0, 16, 0, 0, 0, 0);

  // store the surface
  _surfaces.push(SSurface());
  SSurface &NewSurface = _surfaces[_surfaces.size()-1];
  NewSurface.count = 1;
  const int FilenameLen(strlen(filename));
  NewSurface.filename = (char*)malloc(FilenameLen * sizeof(char));
  strcpy(NewSurface.filename, filename);
  NewSurface.surface = Map;

  return Map;
}

SDL_Surface *SurfacesManager::getSurface(const SDL_Surface *surface)
{
  // lookup for existing surface
  for (int i = _surfaces.size(); --i >= 0; )
    if (surface == _surfaces[i].surface)
    {
      ++_surfaces[i].count;
      return _surfaces[i].surface;
    }
  return NULL;
}

void SurfacesManager::freeSurface(const SDL_Surface *surface)
{
  // retrieve the surface
  for (int i = _surfaces.size(); --i >= 0; )
    if (surface == _surfaces[i].surface)
      if (!--_surfaces[i].count)
      {
        fprintf(stderr, "SurfacesManager::freeSurface %s\n", _surfaces[i].filename);
        SDL_FreeSurface(_surfaces[i].surface);
        free(_surfaces[i].filename);
        _surfaces.remove(i);
        return;
      }
}
