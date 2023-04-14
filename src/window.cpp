#include "window.h"

Chi8P::Window::Window() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    CERR(MSG_ERRSDLI); exit(2);
  }
  if (!(_Window = SDL_CreateWindow(MSG_PGTITLE,
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN))
  ) {
    CERR(MSG_ERRWNDI); Window::~Window(); exit(3);
  }
  if (!(Renderer = SDL_CreateRenderer(_Window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
  ) {
    CERR(MSG_ERRRNDI); Window::~Window(); exit(4);
  }
}

Chi8P::Window::~Window() {
  if (Renderer) SDL_DestroyRenderer(Renderer);
  if (_Window) SDL_DestroyWindow(_Window);
  SDL_Quit();
}

// https://chat.openai.com/c/774986b3-3aea-4514-9d15-17ea65634536
void Chi8P::Window::clear() {
  SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
  SDL_RenderClear(Renderer);
  SDL_RenderPresent(Renderer);
}