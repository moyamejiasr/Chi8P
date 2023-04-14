#pragma once
#include <array>
#include <functional>
#include "common.h"
#include "memory.h"
#include "window.h"
using namespace std::placeholders;

class Chi8P::Processor {
  Memory* p_Memory;
  Window* p_Window;

  void sys_op(unsigned short);
  void jmp_op(unsigned short);
  void cll_op(unsigned short);
  void sec_op(unsigned short);
  void snc_op(unsigned short);
  void ser_op(unsigned short);
  void ldc_op(unsigned short);
  std::array<Instruction, 0xF> _OpCodes {
    // Movement
    &Processor::sys_op, &Processor::jmp_op, &Processor::cll_op,
    // Logical
    &Processor::sec_op, &Processor::snc_op, &Processor::ser_op,
    // Storage
    &Processor::ldc_op
  };
public:
  Processor(Memory*, Window*);
  void execute(unsigned short);
};