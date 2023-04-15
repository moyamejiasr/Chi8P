#pragma once
#include <array>
#include <functional>
#include "common.h"
#include "memory.h"
#include "window.h"
using namespace std::placeholders;

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
  Memory* p_Memory;
  Window* p_Window;

  void sys_op(unsigned short); // 0
  void jpc_op(unsigned short); // 1
  void cll_op(unsigned short); // 2
  void sec_op(unsigned short); // 3
  void snc_op(unsigned short); // 4
  void ser_op(unsigned short); // 5
  void ldc_op(unsigned short); // 6
  void adc_op(unsigned short); // 7
  void ldr_op(unsigned short); // 8
  void snr_op(unsigned short); // 9
  void ldi_op(unsigned short); // A
  void jpr_op(unsigned short); // B
  std::array<Instruction, 0xF> _OpCodes {
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
    &Processor::ldi_op
  };
public:
  Processor(Memory*, Window*);
  void execute(unsigned short);
};