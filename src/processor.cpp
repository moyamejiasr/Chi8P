#include "processor.h"

Chi8P::Processor::Processor(Memory* memory, Window* wnd) {
  this->memory = memory; this->window = wnd;
}

void Chi8P::Processor::sys_op(unsigned short opcode) {
  switch (opcode) {
    case 0x00E0: memory->clearfb(); break;
    case 0x00EE: memory->jump(memory->pop()); break;
    default: jpc_op(opcode);
  }
}

void Chi8P::Processor::jpc_op(unsigned short opcode) {
  memory->jump(V_ADDRESS(opcode));
}

void Chi8P::Processor::cll_op(unsigned short opcode) {
  memory->push(memory->getpc() + sizeof(unsigned short));
  jpc_op(opcode);
}

void Chi8P::Processor::sec_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  if (value == SECOND(opcode)) {
    memory->step();
  }
}

void Chi8P::Processor::snc_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  if (value != SECOND(opcode)) {
    memory->step();
  }
}

void Chi8P::Processor::ser_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  auto value2 = memory->getv(SECOND_H(opcode));
  if (value == value2) {
    memory->step();
  }
}

void Chi8P::Processor::ldc_op(unsigned short opcode) {
  memory->setv(FIRST_L(opcode), SECOND(opcode));
}

void Chi8P::Processor::adc_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  memory->setv(FIRST_L(opcode), value + SECOND(opcode));
}

void Chi8P::Processor::ldr_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  auto value2 = memory->getv(SECOND_H(opcode));
  switch (SECOND_L(opcode)) {
    case 0x0: memory->setv(FIRST_L(opcode), value2); break;
    case 0x1: memory->setv(FIRST_L(opcode), value | value2); break;
    case 0x2: memory->setv(FIRST_L(opcode), value & value2); break;
    case 0x3: memory->setv(FIRST_L(opcode), value ^ value2); break;
    case 0x4: {
      // Only way I can check for overflow
      unsigned short result = static_cast<unsigned short>(value)
        + static_cast<unsigned short>(value2);
      memory->setv(0xF, (result & 0xFF00) != 0);
      memory->setv(FIRST_L(opcode), result);
      break;
    }
    case 0x5: {
      memory->setv(0xF, !(value2 > value));
      memory->setv(FIRST_L(opcode), value - value2);
      break;
    }
    case 0x6: {
      memory->setv(0xF, value & 0x01); // 0000 0001
      memory->setv(FIRST_L(opcode), value >> 1);
      break;
    }
    case 0x7: {
      memory->setv(0xF, !(value > value2));
      memory->setv(FIRST_L(opcode), value2 - value);
      break;
    }
    case 0xE: {
      memory->setv(0xF, (value & 0x80) != 0); // 1000 0000
      memory->setv(FIRST_L(opcode), value << 1);
      break;
    }
  }
}

void Chi8P::Processor::snr_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  auto value2 = memory->getv(SECOND_H(opcode));
  if (value != value2) {
    memory->step();
  }
}

void Chi8P::Processor::ldi_op(unsigned short opcode) {
  memory->seti(V_ADDRESS(opcode));
}

void Chi8P::Processor::jpr_op(unsigned short opcode) {
  auto value = memory->getv(0x0);
  jpc_op(V_ADDRESS(opcode) + value);
}

void Chi8P::Processor::rnd_op(unsigned short opcode) {
  auto value = (std::rand() % 256) & SECOND(opcode);
  memory->setv(FIRST_L(opcode), value);
}

void Chi8P::Processor::dsp_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  auto value2 = memory->getv(SECOND_H(opcode));
  auto height = SECOND_L(opcode);

  auto I = memory->geti();
  for (int row = 0; row < height; row++) {
    auto sprite_row = memory->read(I + row);

    for (int col = 0; col < 8; col++) {
      // Get pixel activation / deactivation
      unsigned char sprite_pixel = (sprite_row >> (7 - col)) & 0x1;
      // Get pixel position and reset it on overflow
      auto x = (value + col) % FRAMEBUFFER_WIDTH,
        y = (value2 + row) % FRAMEBUFFER_HEIGHT;

      // Convert them to byte_index and bit_offset
      int framebuffer_index = (y * FRAMEBUFFER_WIDTH + x) / 8;
      int bit_position = 7 - (x % 8);
      memory->setfb(framebuffer_index, bit_position, sprite_pixel);
    }
  }
}

void Chi8P::Processor::skp_op(unsigned short opcode) {
  auto value = memory->getv(FIRST_L(opcode));
  switch (SECOND(opcode)) {
    case 0x9E: {
      if (window->ispressed(value)) {
        memory->step();
      }
      break;
    }
    case 0xA1: {
      if (!window->ispressed(value)) {
        memory->step();
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
      memory->setv(v, window->getchar());
      break;
    }
    case 0x29: {
      memory->seti(memory->getv(v) * 5); // Length of a sprite in rows
      break;
    }
    case 0x33: {
      auto I = memory->geti();
      memory->write(I, memory->getv(v) / 100);
      memory->write(I + 1, memory->getv(v) % 100 / 10);
      memory->write(I + 2, memory->getv(v) % 10);
      break;
    }
    case 0x65: {
      auto I = memory->geti();
      for (unsigned char i = 0; i <= v; i++) {
        memory->setv(i, memory->read(I + i));
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