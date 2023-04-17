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
  if (!(Renderer = SDL_CreateRenderer(_Window, -1,
    SDL_RENDERER_ACCELERATED))
  ) {
    CERR(MSG_ERRRNDI); this->~Window(); exit(4);
  }
  // A one-bit-per-pixel Surface, indexed to these colors
  if (!(Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT))
  ) {
    CERR(MSG_ERRSFDI); this->~Window(); exit(5);
  }
}

Chi8P::Window::~Window() {
  if (Texture) {
    SDL_DestroyTexture(Texture);
    Texture = nullptr;
  }
  if (Renderer) {
    SDL_DestroyRenderer(Renderer);
    Renderer = nullptr;
  }
  if (_Window) {
    SDL_DestroyWindow(_Window);
    _Window = nullptr;
  }
  SDL_Quit();
}

void Chi8P::Window::draw(unsigned char* buffer) {
  unsigned* texture_buffer;
  int texture_pitch;
  SDL_LockTexture(Texture, NULL, (void**)&texture_buffer, &texture_pitch);
  for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; ++i) {
    // Get the bit to render and then check for the color
    unsigned char pixel = (buffer[i / 8] >> (7 - (i % 8))) & 0x1;
    texture_buffer[i] = pixel ? 0xFFFFFFFF : 0x00000000;
  }
  SDL_UnlockTexture(Texture);

  // Render the texture
  SDL_RenderClear(Renderer);
  SDL_RenderCopy(Renderer, Texture, NULL, NULL);
  SDL_RenderPresent(Renderer);
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