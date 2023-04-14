#pragma once
#include <array>
#include <functional>
#include "common.h"
#include "memory.h"

class Chi8P::Processor {
  static std::array<Instruction, 0xF> _OpCodes;
  static void sys_op(Memory&, unsigned short);
  static void jmp_op(Memory&, unsigned short);
public:
  void execute(Memory&, unsigned short);
};