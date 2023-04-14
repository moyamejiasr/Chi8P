#include "memory.h"

Chi8P::Memory::Memory() {

}

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

unsigned short Chi8P::Memory::pop() {
  return this->Stack[StackPointer--];
}

void Chi8P::Memory::push(unsigned short data) {
  this->Stack[++StackPointer] = data;
}

unsigned char Chi8P::Memory::getv(unsigned char p) {
  return this->V[p];
}

void Chi8P::Memory::setv(unsigned char p, unsigned char value) {
  this->V[p] = value;
}

unsigned short Chi8P::Memory::pc() {
  return this->ProgramCounter;
}

unsigned short Chi8P::Memory::step() {
  if (!this->valid(ProgramCounter))
    return 0;
  return static_cast<unsigned short>(
    (this->read(ProgramCounter++) << 8) | this->read(ProgramCounter++)
  );
}

void Chi8P::Memory::jump(unsigned short address) {
  this->ProgramCounter = address;
}