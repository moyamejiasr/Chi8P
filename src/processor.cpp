#include "processor.h"

template <typename Derived>
std::array<Chi8P::Instruction<Derived>*, 0xF> Chi8P::Instruction<Derived>::_OpCodes;

void Chi8P::execute(Memory& memory, unsigned short opcode) {
  unsigned char operation = ((opcode >> 8) & 0xFF) >> 4;
  auto codes = Instruction<SYInstruction>::_OpCodes;
  Instruction<SYInstruction>::_OpCodes[operation]->process(memory, opcode);
}

template class Chi8P::Instruction<Chi8P::SYInstruction>;
void Chi8P::SYInstruction::process(Memory& memory, unsigned short opcode) {
  exit(2);
}

template class Chi8P::Instruction<Chi8P::JPInstruction>;
void Chi8P::JPInstruction::process(Memory& memory, unsigned short opcode) {
  unsigned short address = opcode << 4;
  COUT("Jumping to 0x" << std::hex << address);
}