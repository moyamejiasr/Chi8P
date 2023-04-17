#pragma once
#include <SDL.h>
#include <cstring>
#include <unordered_map>
#include "common.h"

class Chi8P::Window {
  SDL_Window *_Window;
  SDL_Renderer* Renderer;
  SDL_Texture* Texture;

public:
  Window();
  ~Window();

  void draw(unsigned char*);

  bool ispressed(unsigned char);
  unsigned char getchar();
};