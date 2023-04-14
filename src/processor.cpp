#include "processor.h"

std::array<Chi8P::Instruction, 0xF> Chi8P::Processor::_OpCodes {
  sys_op, jmp_op
};

void Chi8P::Processor::sys_op(Memory& memory, unsigned short opcode) {
  switch(opcode) {
    case 0x00E0: // CLS
      break;
    case 0x00EE: // RET
      break;
  }
  COUT(MSG_ERRNOOP);
}

void Chi8P::Processor::jmp_op(Memory& memory, unsigned short opcode) {
  memory.jump(opcode & 0x0FFF);
}

void Chi8P::Processor::execute(Memory& memory, unsigned short opcode) {
  unsigned char operation = ((opcode >> 8) & 0xFF) >> 4;
  CLOG(MSG_DBGEXEC << static_cast<int>(operation));
  _OpCodes[operation](memory, opcode);
}