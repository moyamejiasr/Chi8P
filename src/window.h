#pragma once
#include <SDL.h>
#include <algorithm>
#include "common.h"

class Chi8P::Window {
  SDL_Window *_Window;
  SDL_Renderer* Renderer;
  SDL_Surface* Surface;

public:
  Window();
  ~Window();

  void clear();
  void draw(unsigned char*);
  bool key_pressed(unsigned char*);
};