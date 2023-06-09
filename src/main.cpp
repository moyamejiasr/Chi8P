// Copyright 2023 Ricardo Moya Mejias
#include <fstream>
#include <algorithm>
#include "./window.h"
#include "./memory.h"
#include "./processor.h"

using Chi8P::Memory;
using Chi8P::Window;
using Chi8P::Processor;

class Emulator {
  Memory memory;
  Window window;
  Processor processor;

 public:
  explicit Emulator(const std::string& path = "")
    : processor(&memory, &window) {
    if (path.empty()) return;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
      CERR(MSG_ERROPEN);
      exit(1);
    }
    // Default-constructed std::istreambuf_iterator<char>.
    // Acts as an end-of-file marker.
    std::copy(std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>(), memory.data());
    file.close();
  }

  void join() {
    // First op jumps to beginning
    uint16_t opcode = 0x1200;
    while (opcode != 0) {
      processor.execute(opcode);
      window.draw(memory.getfb());
      opcode = memory.step();
    }
    CLOG(MSG_DBGEOPG);
    std::getchar();
  }
};

int main() {
    std::cout << MSG_WELCOME << std::endl;
    COUT(MSG_STARTUP);
    std::srand(std::time(nullptr));
    Emulator emulator("./demo/random.ch8");
    COUT(MSG_PROCEED);
    emulator.join();
    return 0;
}
