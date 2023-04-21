// Copyright 2023 Ricardo Moya Mejias
#pragma once
#include <cstddef>
#include <cstring>
#include "./common.h"

class Chi8P::Memory {
 private:
  union {  // 4096 (4KB)
    uint8_t _Memory[0x1000] {
      // Font Set Definition
      // https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
      0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
      0x20, 0x60, 0x20, 0x20, 0x70,  // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
      0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
      0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
      0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
      0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
      0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };
    struct {
      const uint8_t FontSet[0x80];
      uint8_t FrameBuffer[0x100];  // 64*32/8
      uint8_t Reserved[0x60];  // Not used
      uint16_t Stack[4];
      uint8_t V[16];
      uint8_t DelayTimer;
      uint8_t SoundTimer;
      uint8_t StackPointer;
      uint16_t IndexRegister;
      uint16_t ProgramCounter;
      // Here Be Dragons
      uint8_t ProgramSpace[0xE00];
    };
  };

 public:
  Memory();

  // Basic Access Ops
  uint8_t* data();

  // Memory Ops
  bool valid(uint16_t);
  uint8_t read(uint16_t);
  void write(uint16_t, uint8_t);

  // Frame Buffer Ops
  void clearfb();
  uint8_t* getfb();
  void setfb(uint8_t, uint8_t, uint8_t pixel);

  // Stack Ops
  uint16_t pop();
  void push(uint16_t);

  // Register Ops
  uint16_t geti();
  void seti(uint16_t);
  uint8_t getv(uint8_t);
  void setv(uint8_t, uint8_t);

  // System Ops
  uint16_t getpc();
  uint16_t step();
  void jump(uint16_t);
};

static_assert(sizeof(Chi8P::Memory) == 0x1000,
  "Program memory space does not follow standards");
