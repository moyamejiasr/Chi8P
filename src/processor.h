#pragma once
#include <array>
#include "common.h"

namespace Chi8P {
  void execute(Memory&, unsigned short);

  template <typename Derived>
  class Instruction {
    static Derived& Singleton;
    static std::array<Instruction*, 0xF> _OpCodes;
  protected:
    Instruction(unsigned char c) {
      _OpCodes[c] = this;
    }
    virtual void process(Memory&, unsigned short) = 0;
    friend void execute(Memory&, unsigned short);
  };

  class SYInstruction : Instruction<SYInstruction> {
    SYInstruction() : Instruction(0x0) {};
    void process(Memory&, unsigned short) override;
  };

  class JPInstruction : Instruction<JPInstruction> {
    JPInstruction() : Instruction(0x1) {};
    void process(Memory&, unsigned short) override;
  };
}
