// Copyright 2023 Ricardo Moya Mejias
#include "processor.h"  // NOLINT(build/include_subdir)

Chi8P::Processor::Processor(Memory* memory, Window* wnd) {
  this->memory_ptr = memory;
  this->window_ptr = wnd;
}

void Chi8P::Processor::sys_op(uint16_t opcode) {
  switch (opcode) {
    case 0x00E0: memory_ptr->clearfb(); break;
    case 0x00EE: memory_ptr->jump(memory_ptr->pop()); break;
    default: jpc_op(opcode);
  }
}

void Chi8P::Processor::jpc_op(uint16_t opcode) {
  memory_ptr->jump(V_ADDRESS(opcode));
}

void Chi8P::Processor::cll_op(uint16_t opcode) {
  memory_ptr->push(memory_ptr->getpc() + sizeof(uint16_t));
  jpc_op(opcode);
}

void Chi8P::Processor::sec_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  if (value == SECOND(opcode)) {
    memory_ptr->step();
  }
}

void Chi8P::Processor::snc_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  if (value != SECOND(opcode)) {
    memory_ptr->step();
  }
}

void Chi8P::Processor::ser_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  auto value2 = memory_ptr->getv(SECOND_H(opcode));
  if (value == value2) {
    memory_ptr->step();
  }
}

void Chi8P::Processor::ldc_op(uint16_t opcode) {
  memory_ptr->setv(FIRST_L(opcode), SECOND(opcode));
}

void Chi8P::Processor::adc_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  memory_ptr->setv(FIRST_L(opcode), value + SECOND(opcode));
}

void Chi8P::Processor::ldr_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  auto value2 = memory_ptr->getv(SECOND_H(opcode));
  switch (SECOND_L(opcode)) {
    case 0x0: memory_ptr->setv(FIRST_L(opcode), value2); break;
    case 0x1: memory_ptr->setv(FIRST_L(opcode), value | value2); break;
    case 0x2: memory_ptr->setv(FIRST_L(opcode), value & value2); break;
    case 0x3: memory_ptr->setv(FIRST_L(opcode), value ^ value2); break;
    case 0x4: {
      // Only way I can check for overflow
      uint16_t result = static_cast<uint16_t>(value)
        + static_cast<uint16_t>(value2);
      memory_ptr->setv(0xF, (result & 0xFF00) != 0);
      memory_ptr->setv(FIRST_L(opcode), result);
      break;
    }
    case 0x5: {
      memory_ptr->setv(0xF, !(value2 > value));
      memory_ptr->setv(FIRST_L(opcode), value - value2);
      break;
    }
    case 0x6: {
      memory_ptr->setv(0xF, value & 0x01);  // 0000 0001
      memory_ptr->setv(FIRST_L(opcode), value >> 1);
      break;
    }
    case 0x7: {
      memory_ptr->setv(0xF, !(value > value2));
      memory_ptr->setv(FIRST_L(opcode), value2 - value);
      break;
    }
    case 0xE: {
      memory_ptr->setv(0xF, (value & 0x80) != 0);  // 1000 0000
      memory_ptr->setv(FIRST_L(opcode), value << 1);
      break;
    }
  }
}

void Chi8P::Processor::snr_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  auto value2 = memory_ptr->getv(SECOND_H(opcode));
  if (value != value2) {
    memory_ptr->step();
  }
}

void Chi8P::Processor::ldi_op(uint16_t opcode) {
  memory_ptr->seti(V_ADDRESS(opcode));
}

void Chi8P::Processor::jpr_op(uint16_t opcode) {
  auto value = memory_ptr->getv(0x0);
  jpc_op(V_ADDRESS(opcode) + value);
}

void Chi8P::Processor::rnd_op(uint16_t opcode) {
  auto value = (std::rand() % 256) & SECOND(opcode);
  memory_ptr->setv(FIRST_L(opcode), value);
}

void Chi8P::Processor::dsp_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  auto value2 = memory_ptr->getv(SECOND_H(opcode));
  auto height = SECOND_L(opcode);

  auto I = memory_ptr->geti();
  for (int row = 0; row < height; row++) {
    auto sprite_row = memory_ptr->read(I + row);

    for (int col = 0; col < 8; col++) {
      // Get pixel activation / deactivation
      uint8_t sprite_pixel = (sprite_row >> (7 - col)) & 0x1;
      // Get pixel position and reset it on overflow
      auto x = (value + col) % FRAMEBUFFER_WIDTH,
        y = (value2 + row) % FRAMEBUFFER_HEIGHT;

      // Convert them to byte_index and bit_offset
      int framebuffer_index = (y * FRAMEBUFFER_WIDTH + x) / 8;
      int bit_position = 7 - (x % 8);
      memory_ptr->setfb(framebuffer_index, bit_position, sprite_pixel);
    }
  }
}

void Chi8P::Processor::skp_op(uint16_t opcode) {
  auto value = memory_ptr->getv(FIRST_L(opcode));
  switch (SECOND(opcode)) {
    case 0x9E: {
      if (window_ptr->ispressed(value)) {
        memory_ptr->step();
      }
      break;
    }
    case 0xA1: {
      if (!window_ptr->ispressed(value)) {
        memory_ptr->step();
      }
      break;
    }
    default: COUT(MSG_ERRNOOP(opcode));
  }
}

void Chi8P::Processor::ext_op(uint16_t opcode) {
  auto v = FIRST_L(opcode);
  switch (SECOND(opcode)) {
    case 0x0A: {
      memory_ptr->setv(v, window_ptr->getchar());
      break;
    }
    case 0x29: {
      memory_ptr->seti(memory_ptr->getv(v) * 5);  // Length of a sprite in rows
      break;
    }
    case 0x33: {
      auto I = memory_ptr->geti();
      memory_ptr->write(I, memory_ptr->getv(v) / 100);
      memory_ptr->write(I + 1, memory_ptr->getv(v) % 100 / 10);
      memory_ptr->write(I + 2, memory_ptr->getv(v) % 10);
      break;
    }
    case 0x65: {
      auto I = memory_ptr->geti();
      for (uint8_t i = 0; i <= v; i++) {
        memory_ptr->setv(i, memory_ptr->read(I + i));
      }
      break;
    }
    default: COUT(MSG_ERRNOOP(opcode));
  }
}

void Chi8P::Processor::execute(uint16_t opcode) {
  CLOG(MSG_DBGEXEC(opcode));
  (this->*_OpCodes[FIRST_H(opcode)])(opcode);
}
