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

// 8
TEST_F(ProcessorTest, Test_ldr_op) {
  // 0
  memory->setv(1, 0x0001);
  processor->execute(0x8010);
  EXPECT_EQ(memory->getv(0), 0x0001);

  // 1
  memory->setv(1, 0x0002);
  processor->execute(0x8011);
  EXPECT_EQ(memory->getv(0), 0x0003);

  // 2
  memory->setv(0, 0x0001);
  processor->execute(0x8012);
  EXPECT_EQ(memory->getv(0), 0x0000);

  // 3
  memory->setv(0, 0x0002);
  memory->setv(1, 0x0003);
  processor->execute(0x8013);
  EXPECT_EQ(memory->getv(0), 0x0001);

  // 4
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0001);
  memory->setv(1, 0x0003);
  processor->execute(0x8014);
  EXPECT_EQ(memory->getv(0), 0x0004);
  EXPECT_EQ(memory->getv(0xF), 0x0000);
  /* Overflow */
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x00FF);
  memory->setv(1, 0x0001);
  processor->execute(0x8014);
  EXPECT_EQ(memory->getv(0xF), 0x0001);

  // 5
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0003);
  memory->setv(1, 0x0002);
  processor->execute(0x8015);
  EXPECT_EQ(memory->getv(0), 0x0001);
  EXPECT_EQ(memory->getv(0xF), 0x0001);
  /* Borrow */
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0002);
  memory->setv(1, 0x0003);
  processor->execute(0x8015);
  EXPECT_EQ(memory->getv(0xF), 0x0000);

  // 6
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0004);
  processor->execute(0x8006);
  EXPECT_EQ(memory->getv(0), 0x0002);
  EXPECT_EQ(memory->getv(0xF), 0x0000);
  /* Not exact */
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0005);
  processor->execute(0x8006);
  EXPECT_EQ(memory->getv(0), 0x0002);
  EXPECT_EQ(memory->getv(0xF), 0x0001);

  // 7
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0002);
  memory->setv(1, 0x0003);
  processor->execute(0x8017);
  EXPECT_EQ(memory->getv(0), 0x0001);
  EXPECT_EQ(memory->getv(0xF), 0x0001);
  /* Borrow */
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0003);
  memory->setv(1, 0x0002);
  processor->execute(0x8017);
  EXPECT_EQ(memory->getv(0xF), 0x0000);

  // E
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0004);
  processor->execute(0x800E);
  EXPECT_EQ(memory->getv(0), 0x0008);
  EXPECT_EQ(memory->getv(0xF), 0x0000);
  /* Not exact */
  memory->setv(0xF, 0x0000);
  memory->setv(0, 0x0084);
  processor->execute(0x800E);
  EXPECT_EQ(memory->getv(0), 0x0008);
  EXPECT_EQ(memory->getv(0xF), 0x0001);
}

// 9
TEST_F(ProcessorTest, Test_snr_op) {
  memory->setv(0, 0x12);
  memory->setv(1, 0x12);
  processor->execute(0x9010);
  EXPECT_EQ(memory->getpc(), 0x0000);

  memory->setv(0, 0x11);
  memory->setv(1, 0x12);
  processor->execute(0x9010);
  EXPECT_EQ(memory->getpc(), 0x0002);
}

// A
TEST_F(ProcessorTest, Test_ldi_op) {
  processor->execute(0xA200);
  EXPECT_EQ(memory->geti(), 0x0200);
}

// B
TEST_F(ProcessorTest, Test_jpr_op) {
  memory->setv(0, 0x22);
  processor->execute(0xB200);
  EXPECT_EQ(memory->getpc(), 0x0222);
}

// C
TEST_F(ProcessorTest, Test_rnd_op) {
  std::srand(0);
  processor->execute(0xC0FF);
  EXPECT_EQ(memory->getv(0), 0x67); // From seed 0
}

//
// Skipping D for now
//

//
// Skipping E for now
//

// F
