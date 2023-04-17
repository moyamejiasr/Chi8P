#include "memory.h"

Chi8P::Memory::Memory() {
  // Start before 0 so we begin pushing to 0
  StackPointer = -1;
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

unsigned char* Chi8P::Memory::getfb() {
  return this->FrameBuffer;
}

void Chi8P::Memory::setfb(unsigned char index, unsigned char position) {
  if (this->FrameBuffer[index] & (1 << position)) {
    V[0xF] = 1; // Set the collision flag
  }
  // Set based on buffer index and bit position
  this->FrameBuffer[index] ^= (1 << position);
}

unsigned short Chi8P::Memory::pop() {
  return this->Stack[StackPointer--];
}

void Chi8P::Memory::push(unsigned short data) {
  this->Stack[++StackPointer] = data;
}

unsigned short Chi8P::Memory::geti() {
  return this->IndexRegister;
}

void Chi8P::Memory::seti(unsigned short value) {
  this->IndexRegister = value;
}

unsigned char Chi8P::Memory::getv(unsigned char p) {
  return this->V[p];
}

void Chi8P::Memory::setv(unsigned char p, unsigned char value) {
  this->V[p] = value;
}

unsigned short Chi8P::Memory::getpc() {
  return this->ProgramCounter;
}

unsigned short Chi8P::Memory::step() {
  if (!this->valid(ProgramCounter))
    return 0;
  unsigned short value = static_cast<unsigned short>(
    (this->read(ProgramCounter) << 8) | this->read(ProgramCounter + 1)
  );
  ProgramCounter += 2;
  return value;
}

void Chi8P::Memory::jump(unsigned short address) {
  this->ProgramCounter = address;
}