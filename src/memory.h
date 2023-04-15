#pragma once
#include <cstddef>
#include "common.h"

class Chi8P::Memory {
private:
  union { // 4096 (4KB)
    unsigned char _Memory[0x1000] {
      // Font Set Definition
      // https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    struct {
      const unsigned char FontSet[0x80];
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
  Memory();

  // Basic Access Ops
  unsigned char* data();

  // Memory Ops
  bool valid(unsigned short);
  unsigned char read(unsigned short);
  void write(unsigned short, unsigned char);

  // Frame Buffer Ops
  unsigned char* getfb();
  bool setfb(unsigned short, unsigned char);

  // Stack Ops
  unsigned short pop();
  void push(unsigned short);

  // Register Ops
  unsigned short geti();
  void seti(unsigned short);
  unsigned char getv(unsigned char);
  void setv(unsigned char, unsigned char);

  // System Ops
  unsigned short pc();
  unsigned short step();
  void jump(unsigned short);
};

static_assert(sizeof(Chi8P::Memory) == 0x1000,
  "Program memory space does not follow standards");