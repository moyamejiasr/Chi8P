// Copyright 2023 Ricardo Moya Mejias
#pragma once
#include <array>
#include <functional>
#include "./common.h"
#include "./memory.h"
#include "./window.h"

// Logical bitshift operations
// -------------------------
// | F_H ^ F_L | S_H ^ S_L |
// |     F     |     S     |
// |  _  |     ADDRESS     |
// -------------------------
#define FIRST(X) (X >> 8)
#define FIRST_L(X) (FIRST(X) & 0xF)
#define FIRST_H(X) (FIRST(X) >> 4)
#define SECOND(X) (X & 0xFF)
#define SECOND_L(X) (SECOND(X) & 0xF)
#define SECOND_H(X) (SECOND(X) >> 4)
#define V_ADDRESS(X) (X & 0x0FFF)

class Chi8P::Processor {
  Memory* memory_ptr;
  Window* window_ptr;

  void sys_op(uint16_t);  // 0
  void jpc_op(uint16_t);  // 1
  void cll_op(uint16_t);  // 2
  void sec_op(uint16_t);  // 3
  void snc_op(uint16_t);  // 4
  void ser_op(uint16_t);  // 5
  void ldc_op(uint16_t);  // 6
  void adc_op(uint16_t);  // 7
  void ldr_op(uint16_t);  // 8
  void snr_op(uint16_t);  // 9
  void ldi_op(uint16_t);  // A
  void jpr_op(uint16_t);  // B
  void rnd_op(uint16_t);  // C
  void dsp_op(uint16_t);  // D
  void skp_op(uint16_t);  // E
  void ext_op(uint16_t);  // F

  const std::array<Instruction, 16> _OpCodes {
    // Movement
    &Processor::sys_op, &Processor::jpc_op, &Processor::cll_op,
    // Conditional
    &Processor::sec_op, &Processor::snc_op, &Processor::ser_op,
    // Storage
    &Processor::ldc_op,
    // Arithmetical
    &Processor::adc_op,
    // Storage 2 && Logical
    &Processor::ldr_op,
    // Conditional 2
    &Processor::snr_op,
    // Storage 3
    &Processor::ldi_op,
    // Movement 2
    &Processor::jpr_op,
    // Random
    &Processor::rnd_op,
    // Display
    &Processor::dsp_op,
    // Keyboard Condition
    &Processor::skp_op,
    // Extended
    &Processor::ext_op
  };

 public:
  Processor(Memory*, Window*);
  void execute(uint16_t);
};
