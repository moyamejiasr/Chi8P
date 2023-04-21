// Copyright 2023 Ricardo Moya Mejias
#include "window.h"  // NOLINT(build/include_subdir)

Chi8P::Window::Window() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    CERR(MSG_ERRSDLI);
    exit(2);
  }
  if (!(sdl_window_ptr = SDL_CreateWindow(MSG_PGTITLE,
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN))
  ) {
    CERR(MSG_ERRWNDI);
    exit(3);
  }
  if (!(sdl_renderer_ptr = SDL_CreateRenderer(sdl_window_ptr, -1,
    SDL_RENDERER_ACCELERATED))
  ) {
    CERR(MSG_ERRRNDI);
    exit(4);
  }
  // A one-bit-per-pixel Surface, indexed to these colors
  if (!(sdl_texture_ptr = SDL_CreateTexture(sdl_renderer_ptr,
    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
    FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT))
  ) {
    CERR(MSG_ERRTXDI);
    exit(5);
  }
}

Chi8P::Window::~Window() {
  if (sdl_texture_ptr) {
    SDL_DestroyTexture(sdl_texture_ptr);
    sdl_texture_ptr = nullptr;
  }
  if (sdl_renderer_ptr) {
    SDL_DestroyRenderer(sdl_renderer_ptr);
    sdl_renderer_ptr = nullptr;
  }
  if (sdl_window_ptr) {
    SDL_DestroyWindow(sdl_window_ptr);
    sdl_window_ptr = nullptr;
  }
  SDL_Quit();
}

void Chi8P::Window::draw(uint8_t* buffer) {
  unsigned* texture_buffer;
  int texture_pitch;
  SDL_LockTexture(sdl_texture_ptr, NULL,
    reinterpret_cast<void**>(&texture_buffer), &texture_pitch);
  for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; ++i) {
    // Get the bit to render and then check for the color
    uint8_t pixel = (buffer[i / 8] >> (7 - (i % 8))) & 0x1;
    texture_buffer[i] = pixel ? 0xFFFFFFFF : 0x00000000;
  }
  SDL_UnlockTexture(sdl_texture_ptr);

  // Render the texture
  SDL_RenderClear(sdl_renderer_ptr);
  SDL_RenderCopy(sdl_renderer_ptr, sdl_texture_ptr, NULL, NULL);
  SDL_RenderPresent(sdl_renderer_ptr);
}

bool Chi8P::Window::ispressed(uint8_t key) {
  static std::unordered_map<uint8_t, SDL_Scancode> key_map {
    {0x0, SDL_SCANCODE_0}, {0x1, SDL_SCANCODE_1}, {0x2, SDL_SCANCODE_2},
    {0x3, SDL_SCANCODE_3}, {0x4, SDL_SCANCODE_4}, {0x5, SDL_SCANCODE_5},
    {0x6, SDL_SCANCODE_6}, {0x7, SDL_SCANCODE_7}, {0x8, SDL_SCANCODE_8},
    {0x9, SDL_SCANCODE_9},
    {0xA, SDL_SCANCODE_A}, {0xB, SDL_SCANCODE_B}, {0xC, SDL_SCANCODE_C},
    {0xD, SDL_SCANCODE_D}, {0xE, SDL_SCANCODE_E}, {0xF, SDL_SCANCODE_F}
  };
  auto state = SDL_GetKeyboardState(nullptr);
  return state[key_map[key]];
}

uint8_t Chi8P::Window::getchar() {
  static std::unordered_map<SDL_Scancode, uint8_t> key_map {
    {SDL_SCANCODE_0, 0x0}, {SDL_SCANCODE_1, 0x1}, {SDL_SCANCODE_2, 0x2},
    {SDL_SCANCODE_3, 0x3}, {SDL_SCANCODE_4, 0x4}, {SDL_SCANCODE_5, 0x5},
    {SDL_SCANCODE_6, 0x6}, {SDL_SCANCODE_7, 0x7}, {SDL_SCANCODE_8, 0x8},
    {SDL_SCANCODE_9, 0x9},
    {SDL_SCANCODE_A, 0xA}, {SDL_SCANCODE_B, 0xB}, {SDL_SCANCODE_C, 0xC},
    {SDL_SCANCODE_D, 0xD}, {SDL_SCANCODE_E, 0xE}, {SDL_SCANCODE_F, 0xF}
  };
  SDL_Event event;
  while (true) {
    SDL_WaitEvent(&event);
    if (event.type != SDL_KEYDOWN)
      continue;
    // Parse
    auto it = key_map.find(event.key.keysym.scancode);
    if (it == key_map.end())
      continue;
    return it->second;
  }
}
