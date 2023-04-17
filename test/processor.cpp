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

// 0
TEST_F(ProcessorTest, Test_sys_op) {
  processor->execute(0x0200);
  EXPECT_EQ(memory->getpc(), 0x0200);

  processor->execute(0x00E0);
  EXPECT_EQ(memory->getpc(), 0x0200); // No jmp

  memory->push(0x0234);
  processor->execute(0x00EE);
  EXPECT_EQ(memory->getpc(), 0x0234);
}

// 1
TEST_F(ProcessorTest, Test_jpc_op) {
  processor->execute(0x1200);
  EXPECT_EQ(memory->getpc(), 0x0200);
}

// 2
TEST_F(ProcessorTest, Test_cll_op) {
  processor->execute(0x2200);
  EXPECT_EQ(memory->getpc(), 0x0200);
  EXPECT_EQ(memory->pop(), 0x0002);
}

// 3
TEST_F(ProcessorTest, Test_sec_op) {
  memory->setv(0, 0x11);
  processor->execute(0x3012);
  EXPECT_EQ(memory->getpc(), 0x0000);

  memory->setv(0, 0x12);
  processor->execute(0x3012);
  EXPECT_EQ(memory->getpc(), 0x0002);
}

// 4
TEST_F(ProcessorTest, Test_snc_op) {
  memory->setv(0, 0x12);
  processor->execute(0x4012);
  EXPECT_EQ(memory->getpc(), 0x0000);

  memory->setv(0, 0x11);
  processor->execute(0x4012);
  EXPECT_EQ(memory->getpc(), 0x0002);
}

// 5
TEST_F(ProcessorTest, Test_ser_op) {
  memory->setv(0, 0x11);
  memory->setv(1, 0x12);
  processor->execute(0x5010);
  EXPECT_EQ(memory->getpc(), 0x0000);

  memory->setv(0, 0x12);
  memory->setv(1, 0x12);
  processor->execute(0x5010);
  EXPECT_EQ(memory->getpc(), 0x0002);
}

// 6
TEST_F(ProcessorTest, Test_ldc_op) {
  processor->execute(0x6012);
  EXPECT_EQ(memory->getv(0), 0x0012);
}

// 7
TEST_F(ProcessorTest, Test_adc_op) {
  memory->setv(0, 0x0001);
  processor->execute(0x7012);
  EXPECT_EQ(memory->getv(0), 0x0013);
}