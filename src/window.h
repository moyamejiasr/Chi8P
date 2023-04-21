// Copyright 2023 Ricardo Moya Mejias
#pragma once
#include <SDL.h>
#include <cstring>
#include <unordered_map>
#include "./common.h"

class Chi8P::Window {
  SDL_Window *sdl_window_ptr;
  SDL_Renderer* sdl_renderer_ptr;
  SDL_Texture* sdl_texture_ptr;

 public:
  Window();
  ~Window();

  void draw(uint8_t*);

  bool ispressed(uint8_t);
  uint8_t getchar();
};
