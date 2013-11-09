#ifndef __SURFACESMANAGER_H__
#define __SURFACESMANAGER_H__

#include <SDL.h>

#include "XVector.h"

class SurfacesManager
{
  typedef struct Surface
  {
    unsigned int count{0};
    SDL_Surface *surface{NULL};
    char *filename{(char*)"\0"};
  } SSurface;

  public:
    SurfacesManager() {}

    static SDL_Surface *loadImage(const char *filename);
    static SDL_Surface *getSurface(const SDL_Surface *surface);
    static void freeSurface(const SDL_Surface *surface);

  protected:
    static XVector<SSurface> _surfaces;
};

#endif
