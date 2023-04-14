#pragma once
#include <cstddef>
#include "common.h"

class Chi8P::Memory {
private:
  union { // 4096 (4KB)
    unsigned char _Memory[0x1000] {};
    struct {
      unsigned char FontSet[0x80];
      unsigned char FrameBuffer[0x100]; // 64*32/8
      unsigned char Reserved[0x60]; // Not used
      unsigned char Stack[8];
      unsigned char V[16];
      unsigned char DelayTimer;
      unsigned char SoundTimer;
      unsigned char StackPointer;
      unsigned short IndexRegister;
      unsigned short ProgramCounter;
      // Here Be Dragons
      unsigned char ProgramSpace[0xE00];
    };
  };
public:
  // Basic Access Ops
  unsigned char* data();

  // Memory Ops
  bool valid(unsigned short);
  unsigned char read(unsigned short);
  void write(unsigned short, unsigned char);

  // System Ops
  unsigned short step();
  void jump(unsigned short);
};

static_assert(sizeof(Chi8P::Memory) == 0x1000,
  "Program memory space does not follow standards");