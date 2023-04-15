#include "processor.h"

Chi8P::Processor::Processor(Memory* memory, Window* wnd) {
  this->p_Memory = memory; this->p_Window = wnd;
}

void Chi8P::Processor::sys_op(unsigned short opcode) {
  switch(opcode) {
    case 0x00E0: p_Window->clear(); break;
    case 0x00EE: p_Memory->jump(p_Memory->pop()); break;
    default: jpc_op(opcode);
  }
}

void Chi8P::Processor::jpc_op(unsigned short opcode) {
  p_Memory->jump(V_ADDRESS(opcode));
}

void Chi8P::Processor::cll_op(unsigned short opcode) {
  p_Memory->push(p_Memory->pc());
  jpc_op(opcode);
}

void Chi8P::Processor::sec_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  if (value == SECOND(opcode)) {
    p_Memory->step();
  }
}

void Chi8P::Processor::snc_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  if (value != SECOND(opcode)) {
    p_Memory->step();
  }
}

void Chi8P::Processor::ser_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  auto value2 = p_Memory->getv(SECOND_H(opcode));
  if (value == value2) {
    p_Memory->step();
  }
}

void Chi8P::Processor::ldc_op(unsigned short opcode) {
  p_Memory->setv(FIRST_L(opcode), SECOND(opcode));
}

void Chi8P::Processor::adc_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  p_Memory->setv(FIRST_L(opcode), value + SECOND(opcode));
}

void Chi8P::Processor::ldr_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  auto value2 = p_Memory->getv(SECOND_H(opcode));
  switch (SECOND_L(opcode)) {
    case 0x0: p_Memory->setv(FIRST_L(opcode), value2); break;
    case 0x1: p_Memory->setv(FIRST_L(opcode), value | value2); break;
    case 0x2: p_Memory->setv(FIRST_L(opcode), value & value2); break;
    case 0x3: p_Memory->setv(FIRST_L(opcode), value ^ value2); break;
    case 0x4: {
      unsigned short result = static_cast<unsigned short>(value)
        + static_cast<unsigned short>(value2);
      p_Memory->setv(0xF, (result & 0xFF00) != 0);
      p_Memory->setv(FIRST_L(opcode), result);
      break;
    }
    case 0x5: {
      short result = static_cast<short>(value)
        - static_cast<short>(value2);
      p_Memory->setv(0xF, !(result < 0));
      p_Memory->setv(FIRST_L(opcode), result);
      break;
    }
    case 0x6: {
      p_Memory->setv(0xF, value & 0x01); // 0000 0001
      p_Memory->setv(FIRST_L(opcode), value >> 1);
      break;
    }
    case 0x7: {
      short result = static_cast<short>(value2)
        - static_cast<short>(value);
      p_Memory->setv(0xF, !(result < 0));
      p_Memory->setv(FIRST_L(opcode), result);
      break;
    }
    case 0xE: {
      p_Memory->setv(0xF, value & 0x80); // 1000 0000
      p_Memory->setv(FIRST_L(opcode), value << 1);
      break;
    }
  }
}

void Chi8P::Processor::snr_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  auto value2 = p_Memory->getv(SECOND_H(opcode));
  if (value != value2) {
    p_Memory->step();
  }
}

void Chi8P::Processor::ldi_op(unsigned short opcode) {
  p_Memory->seti(V_ADDRESS(opcode));
}

void Chi8P::Processor::jpr_op(unsigned short opcode) {
  auto value = p_Memory->getv(0x0);
  jpc_op(V_ADDRESS(opcode) + value);
}

void Chi8P::Processor::execute(unsigned short opcode) {
  unsigned char operation = FIRST_H(opcode);
  CLOG(MSG_DBGEXEC << static_cast<int>(operation));
  (this->*_OpCodes[operation])(opcode);
}