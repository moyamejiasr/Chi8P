#pragma once
#include <SDL.h>
#include <cstring>
#include <unordered_map>
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

  bool ispressed(unsigned char);
  unsigned char getchar();
};