#pragma once
#include <SDL.h>
#include "common.h"

class Chi8P::Window {
  SDL_Window *_Window;
  SDL_Renderer* Renderer;

public:
  Window();
  ~Window();

  void clear();
};