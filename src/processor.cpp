#include "processor.h"

Chi8P::Processor::Processor(Memory* memory, Window* wnd) {
  this->p_Memory = memory; this->p_Window = wnd;
}

void Chi8P::Processor::sys_op(unsigned short opcode) {
  switch(opcode) {
    case 0x00E0: p_Memory->clearfb(); break;
    case 0x00EE: p_Memory->jump(p_Memory->pop()); break;
    default: jpc_op(opcode);
  }
}

void Chi8P::Processor::jpc_op(unsigned short opcode) {
  p_Memory->jump(V_ADDRESS(opcode));
}

void Chi8P::Processor::cll_op(unsigned short opcode) {
  p_Memory->push(p_Memory->getpc() + sizeof(unsigned short));
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
      // Only way I can check for overflow
      unsigned short result = static_cast<unsigned short>(value)
        + static_cast<unsigned short>(value2);
      p_Memory->setv(0xF, (result & 0xFF00) != 0);
      p_Memory->setv(FIRST_L(opcode), result);
      break;
    }
    case 0x5: {
      p_Memory->setv(0xF, !(value2 > value));
      p_Memory->setv(FIRST_L(opcode), value - value2);
      break;
    }
    case 0x6: {
      p_Memory->setv(0xF, value & 0x01); // 0000 0001
      p_Memory->setv(FIRST_L(opcode), value >> 1);
      break;
    }
    case 0x7: {
      p_Memory->setv(0xF, !(value > value2));
      p_Memory->setv(FIRST_L(opcode), value2 - value);
      break;
    }
    case 0xE: {
      p_Memory->setv(0xF, (value & 0x80) != 0); // 1000 0000
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
  auto value = p_Memory->getv(FIRST_L(opcode));
  auto value2 = p_Memory->getv(SECOND_H(opcode));
  auto height = SECOND_L(opcode);

  auto I = p_Memory->geti();
  for (int row = 0; row < height; row++) {
    auto sprite_row = p_Memory->read(I + row);

    for (int col = 0; col < 8; col++) {
      // Get pixel activation / deactivation
      unsigned char sprite_pixel = (sprite_row >> (7 - col)) & 0x1;
      // Get pixel position and reset it on overflow
      auto x = (value + col) % FRAMEBUFFER_WIDTH,
        y = (value2 + row) % FRAMEBUFFER_HEIGHT;
      
      // Convert them to byte_index and bit_offset
      int framebuffer_index = (y * FRAMEBUFFER_WIDTH + x) / 8;
      int bit_position = 7 - (x % 8);
      p_Memory->setfb(framebuffer_index, bit_position, sprite_pixel);
    }
  }
}

void Chi8P::Processor::skp_op(unsigned short opcode) {
  auto value = p_Memory->getv(FIRST_L(opcode));
  switch (SECOND(opcode)) {
    case 0x9E: {
      if (p_Window->ispressed(value)) {
        p_Memory->step();
      }
      break;
    }
    case 0xA1: {
      if (!p_Window->ispressed(value)) {
        p_Memory->step();
      }
      break;
    }
    default: COUT(MSG_ERRNOOP(opcode));
  }
}

void Chi8P::Processor::ext_op(unsigned short opcode) {
  auto v = FIRST_L(opcode);
  switch (SECOND(opcode)) {
    case 0x0A: {
      p_Memory->setv(v, p_Window->getchar());
      break;
    }
    case 0x29: {
      p_Memory->seti(p_Memory->getv(v) * 5); // Length of a sprite in rows
      break;
    }
    case 0x33: {
      auto I = p_Memory->geti();
      p_Memory->write(I, p_Memory->getv(v) / 100);
      p_Memory->write(I + 1, p_Memory->getv(v) % 100 / 10);
      p_Memory->write(I + 2, p_Memory->getv(v) % 10);
      break;
    }
    case 0x65: {
      auto I = p_Memory->geti();
      for (unsigned char i = 0; i <= v; i++) {
        p_Memory->setv(i, p_Memory->read(I + i));
      }
      break;
    }
    default: COUT(MSG_ERRNOOP(opcode));
  }
}

void Chi8P::Processor::execute(unsigned short opcode) {
  CLOG(MSG_DBGEXEC(opcode));
  (this->*_OpCodes[FIRST_H(opcode)])(opcode);
}