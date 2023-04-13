#include "memory.h"

unsigned char* Chi8P::Memory::data() {
  return this->ProgramSpace;
}

bool Chi8P::Memory::valid(unsigned short address) {
  return address < sizeof(this->_Memory);
}

unsigned char Chi8P::Memory::read(unsigned short address) {
  return this->_Memory[address];
}

void Chi8P::Memory::write(unsigned short address, unsigned char value) {
  this->_Memory[address] = value;
}

unsigned short Chi8P::Memory::step() {
  if (!this->valid(ProgramCounter))
    return 0;
  auto opcode = this->read(ProgramCounter); ProgramCounter += 2;
  return opcode;
}