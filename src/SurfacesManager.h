#pragma once

#include <map>
#include <memory>
#include <string>

#include <SDL3/SDL.h>

class SurfacesManager
{
public:
  using SurfacePtr = std::shared_ptr<SDL_Surface>;

  static SurfacePtr loadImage(const std::string& filename);

protected:
  static std::map<std::string, std::weak_ptr<SDL_Surface>>& surfaces();
};
