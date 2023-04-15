#include "window.h"

Chi8P::Window::Window() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    CERR(MSG_ERRSDLI); exit(2);
  }
  if (!(_Window = SDL_CreateWindow(MSG_PGTITLE,
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN))
  ) {
    CERR(MSG_ERRWNDI); this->~Window(); exit(3);
  }
  if (!(Renderer = SDL_CreateRenderer(_Window, -1, SDL_RENDERER_PRESENTVSYNC))
  ) {
    CERR(MSG_ERRRNDI); this->~Window(); exit(4);
  }

  // Set render scale to make framebuffer fit the whole screen
  SDL_RenderSetScale(Renderer, SCREEN_SCALE, SCREEN_SCALE);

  // Limit the window size so that it cannot be smaller
  // https://stackoverflow.com/questions/55107529/how-to-create-a-1-bit-per-pixel-surface-with-sdl2
  SDL_SetWindowMinimumSize(_Window, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
  SDL_RenderSetLogicalSize(Renderer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
  SDL_RenderSetIntegerScale(Renderer, SDL_TRUE); // Round down to smaller int size

  // A one-bit-per-pixel Surface, indexed to these colors
  Surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE,
    FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 1, SDL_PIXELFORMAT_INDEX1MSB);
  SDL_Color colors[2] = {{0, 0, 0, 255}, {255, 255, 255, 255}};
  SDL_SetPaletteColors(Surface->format->palette, colors, 0, 2);
}

Chi8P::Window::~Window() {
  if (Surface) SDL_FreeSurface(Surface);
  if (Renderer) SDL_DestroyRenderer(Renderer);
  if (_Window) SDL_DestroyWindow(_Window);
  SDL_Quit();
}

void Chi8P::Window::clear() {
  // https://chat.openai.com/c/774986b3-3aea-4514-9d15-17ea65634536
  SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 255);

  SDL_RenderClear(Renderer);
  SDL_RenderPresent(Renderer);
}

void Chi8P::Window::draw(unsigned char* buffer) {
  // Copy the buffer from memory
  SDL_LockSurface(Surface);
  std::memcpy(Surface->pixels, buffer, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT / 8);
  SDL_UnlockSurface(Surface);

  // Draw the Surface to the screen
  // https://stackoverflow.com/questions/55107529/how-to-create-a-1-bit-per-pixel-surface-with-sdl2
  SDL_Texture * screen_texture = SDL_CreateTextureFromSurface(Renderer, Surface);
  
  SDL_RenderClear(Renderer);
  SDL_RenderCopy(Renderer, screen_texture, NULL, NULL);
  SDL_RenderPresent(Renderer);

  SDL_DestroyTexture(screen_texture);
}

bool Chi8P::Window::ispressed(unsigned char key) {
  static std::unordered_map<unsigned char, SDL_Scancode> key_map {
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

unsigned char Chi8P::Window::getchar() {
  static std::unordered_map<SDL_Scancode, unsigned char> key_map {
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