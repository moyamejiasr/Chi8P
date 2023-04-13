#pragma once
#include <array>
#include "memory.h"

class OpCode {
  unsigned char code;
  static std::array<OpCode*, 0xF> _OpCodes;
protected:
  OpCode() { _OpCodes[code] = this; }
  ~OpCode() { _OpCodes[code] = nullptr; }
public:
  virtual void invoke() = 0;

  static const OpCode& Execute(Memory& memory) {
    return _OpCodes[];
  }
};

class SYCode: OpCode {
  unsigned char code = 0x0;
  virtual void invoke() {

  };
} SY;
