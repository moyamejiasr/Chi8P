#include <gtest/gtest.h>
#include "../src/memory.h"
#include "../src/window.h"
#include "../src/processor.h"
using namespace Chi8P;

class ProcessorTest : public ::testing::Test {
protected:
  ProcessorTest() {
    memory = new Memory();
    window = new Window();
    processor = new Processor(memory, window);
  }

  ~ProcessorTest() {
    delete processor;
    delete window;
    processor = nullptr;
  }

  // Initialization
  Memory* memory;
  Window* window;
  Processor* processor;
};

TEST_F(ProcessorTest, Test_sys_op) {
  processor->execute(0x0200);
  EXPECT_EQ(memory->getpc(), 0x0200);
  processor->execute(0x00E0);
  EXPECT_EQ(memory->getpc(), 0x0200); // No jmp
  memory->push(0x0234);
  processor->execute(0x00EE);
  EXPECT_EQ(memory->getpc(), 0x0234);
}
