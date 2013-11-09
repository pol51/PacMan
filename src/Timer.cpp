#include "Timer.h"

#include <SDL_timer.h>

void Timer::Start()
{
  _started = true;
  _startTicks = SDL_GetTicks();
}

bool Timer::IsFPSReached() const
{
  return (_started && _fps && (SDL_GetTicks() - _startTicks) > 1000 / _fps);
}
