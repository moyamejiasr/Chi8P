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

void Chi8P::Processor::rnd_op(unsigned short opcode) {
  auto value = (std::rand() % 256) & SECOND(opcode);
  p_Memory->setv(FIRST_L(opcode), value);
}

void Chi8P::Processor::dsp_op(unsigned short opcode) {
  auto x = p_Memory->getv(FIRST_L(opcode)) % FRAMEBUFFER_WIDTH;
  auto y = p_Memory->getv(SECOND_H(opcode)) % FRAMEBUFFER_HEIGHT;
  auto height = p_Memory->getv(SECOND_L(opcode));

  // 1- Read n-byte-rows from memory starting from I
  auto I = p_Memory->geti();
  for (int i = 0; i < height; i++) {
    auto row = p_Memory->read(I + i); // Sprite Row = I Register + iter
    // 2- Apply row to frame buffer
    for (int j = 0; j < 8; j++) {
      // 3- Get bit as pixel
      uint8_t pixel = row & (0x80 >> j);
      // 4- Proceed if pixel set
      if (pixel != 0) {
        // (YPos + RowIndex) = Current row
        // (y + i) * (FRAMEBUFFER_WIDTH / 8) = Start of the row in the buffer
        // (x + j) = Current column
        // (x + j) / 8 = Byte in the current row that the pixel belongs to
        // Buffer Index = Starting Index Current Row + Byte Index Within Row
        auto offset = (y + i) * (FRAMEBUFFER_WIDTH / 8) + (x + j) / 8;
        auto bit_pos = 7 - (x + j) % 8;
      }
    }
  }
}

void Chi8P::Processor::skp_op(unsigned short opcode) {
}

void Chi8P::Processor::execute(unsigned short opcode) {
  unsigned char operation = FIRST_H(opcode);
  CLOG(MSG_DBGEXEC << static_cast<int>(operation));
  (this->*_OpCodes[operation])(opcode);
}