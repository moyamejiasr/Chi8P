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
  std::array<Instruction, 0xF> _OpCodes {
    std::bind(&Processor::sys_op, this, std::placeholders::_2),
    std::bind(&Processor::jmp_op, this, std::placeholders::_2),
  };
public:
  Processor(Memory*, Window*);
  void execute(unsigned short);
};