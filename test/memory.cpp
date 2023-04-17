#include <gtest/gtest.h>
#include "../src/memory.h"
using namespace Chi8P;

class MemoryTest : public ::testing::Test {
protected:
  MemoryTest() {
    memory = new Memory();
  }

  ~MemoryTest() {
    delete memory;
    memory = nullptr;
  }

  // Initialization
  Memory* memory;
};

TEST_F(MemoryTest, StackPushPop) {
  memory->push(0x0123);
  memory->push(0x4567);
  EXPECT_EQ(memory->pop(), 0x4567);
  EXPECT_EQ(memory->pop(), 0x0123);
  memory->push(0x89AB);
  memory->push(0xCDEF);
  EXPECT_EQ(memory->pop(), 0xCDEF);
  EXPECT_EQ(memory->pop(), 0x89AB);
}