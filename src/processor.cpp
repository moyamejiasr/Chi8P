#include "processor.h"

Chi8P::Processor::Processor(Memory* memory, Window* wnd) {
  this->p_Memory = memory; this->p_Window = wnd;
}

void Chi8P::Processor::sys_op(unsigned short opcode) {
  switch(opcode) {
    case 0x00E0: p_Window->clear(); break;
    case 0x00EE: p_Memory->jump(p_Memory->pop()); break;
    default: jmp_op(opcode);
  }
}

void Chi8P::Processor::jmp_op(unsigned short opcode) {
  p_Memory->jump(opcode & 0x0FFF);
}

void Chi8P::Processor::cll_op(unsigned short opcode) {
  p_Memory->push(p_Memory->pc());
  jmp_op(opcode);
}

void Chi8P::Processor::sec_op(unsigned short opcode) {
  auto value = p_Memory->getv((opcode >> 8) & 0x0F);
  if (value == (opcode & 0xFF)) {
    p_Memory->step();
  }
}

void Chi8P::Processor::snc_op(unsigned short opcode) {
  auto value = p_Memory->getv((opcode >> 8) & 0x0F);
  if (value != (opcode & 0xFF)) {
    p_Memory->step();
  }
}

void Chi8P::Processor::ser_op(unsigned short opcode) {
  auto value = p_Memory->getv((opcode >> 8) & 0x0F);
  auto value2 = p_Memory->getv(opcode & 0x0F);
  if (value == value2) {
    p_Memory->step();
  }
}

void Chi8P::Processor::ldc_op(unsigned short opcode) {
  p_Memory->setv((opcode >> 8) & 0x0F, opcode & 0xFF);
}

void Chi8P::Processor::execute(unsigned short opcode) {
  unsigned char operation = ((opcode >> 8) & 0xFF) >> 4;
  CLOG(MSG_DBGEXEC << static_cast<int>(operation));
  (this->*_OpCodes[operation])(opcode);
}