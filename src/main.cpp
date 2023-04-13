#include <fstream>
#include <iostream>
#include <algorithm>
#include "memory.h"

using namespace Chi8P;
class Emulator {
  Memory memory;

public:
  Emulator(std::string path = "") {
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
  }

  void join() {

  }
};

int main() {
    std::cout << MSG_WELCOME << std::endl;
    COUT(MSG_STARTUP);
    Emulator emulator("demo/random.ch8");
    COUT(MSG_PROCEED);
    emulator.join();
    return 0;
}