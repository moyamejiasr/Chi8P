#include <fstream>
#include <algorithm>
#include "memory.h"
#include "processor.h"

using namespace Chi8P;
class Emulator {
  Memory memory;
public:
  Emulator(std::string path = "") {
    if (path.empty())
      return;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        CERR(MSG_ERROPEN);
        exit(1);
    }
    // Default-constructed std::istreambuf_iterator<char>.
    // Acts as an end-of-file marker.
    std::copy(std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>(), memory.data());
  }

  void join() {
    // First op jumps to beginning
    unsigned short opcode = 0x1200;
    while (opcode != 0) {
      execute(memory, opcode);
      opcode = memory.step();
    }
  }
};

int main() {
    std::cout << MSG_WELCOME << std::endl;
    COUT(MSG_STARTUP);
    Emulator emulator("/home/ricardo/project/user-chip8/demo/random.ch8");
    COUT(MSG_PROCEED);
    emulator.join();
    return 0;
}