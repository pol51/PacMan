#include "SurfacesManager.h"

#include <SDL3_image/SDL_image.h>

std::map<std::string, std::weak_ptr<SDL_Surface>>& SurfacesManager::surfaces()
{
  static std::map<std::string, std::weak_ptr<SDL_Surface>> instance;
  return instance;
}

SurfacesManager::SurfacePtr SurfacesManager::loadImage(const std::string& filename)
{
  // lookup for existing surface
  auto it = surfaces().find(filename);
  if (it != surfaces().end())
    if (auto existing = it->second.lock())
      return existing;

  // load the sdl surface
  SDL_Surface* raw {nullptr};
  if (!filename.empty())
  {
    raw = IMG_Load(filename.c_str());
    if (!raw)
    {
      SDL_Log("Error loading %s", filename.c_str());
      raw = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGB48);
    }
    SDL_Log("SurfacesManager::loadSurface %s", filename.c_str());
  }
  else
    raw = SDL_CreateSurface(0, 0, SDL_PIXELFORMAT_RGB48);

  // store with custom deleter
  auto ptr = SurfacePtr(raw, [filename](SDL_Surface* s) {
    SDL_Log("SurfacesManager::freeSurface %s", filename.c_str());
    SDL_DestroySurface(s);
    surfaces().erase(filename);
  });

  surfaces()[filename] = ptr;
  return ptr;
}
