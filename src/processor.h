#pragma once
#include <array>
#include <functional>
#include "common.h"
#include "memory.h"
#include "window.h"

class Chi8P::Processor {
  Memory* p_Memory;
  Window* p_Window;
  static std::array<Instruction, 0xF> _OpCodes;

  static void sys_op(unsigned short);
  static void jmp_op(unsigned short);
public:
  Processor(Memory*, Window*);
  void execute(unsigned short);
};