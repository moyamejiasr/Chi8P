// Copyright 2023 Ricardo Moya Mejias
#include "memory.h"  // NOLINT(build/include_subdir)

Chi8P::Memory::Memory() {
  // Start before 0 so we begin pushing to 0
  StackPointer = -1;
}

uint8_t* Chi8P::Memory::data() {
  return this->ProgramSpace;
}

bool Chi8P::Memory::valid(uint16_t address) {
  return address < sizeof(this->_Memory);
}

uint8_t Chi8P::Memory::read(uint16_t address) {
  return this->_Memory[address];
}

void Chi8P::Memory::write(uint16_t address, uint8_t value) {
  this->_Memory[address] = value;
}

void Chi8P::Memory::clearfb() {
  std::memset(this->FrameBuffer, 0, sizeof(this->FrameBuffer));
}

uint8_t* Chi8P::Memory::getfb() {
  return this->FrameBuffer;
}

void Chi8P::Memory::setfb(uint8_t index, uint8_t position, uint8_t pixel) {
  if (this->FrameBuffer[index] & (pixel << position)) {
    V[0xF] = 1;  // Set the collision flag
  }
  // Set based on buffer index and bit position
  this->FrameBuffer[index] ^= (pixel << position);
}

uint16_t Chi8P::Memory::pop() {
  return this->Stack[StackPointer--];
}

void Chi8P::Memory::push(uint16_t data) {
  this->Stack[++StackPointer] = data;
}

uint16_t Chi8P::Memory::geti() {
  return this->IndexRegister;
}

void Chi8P::Memory::seti(uint16_t value) {
  this->IndexRegister = value;
}

uint8_t Chi8P::Memory::getv(uint8_t p) {
  return this->V[p];
}

void Chi8P::Memory::setv(uint8_t p, uint8_t value) {
  this->V[p] = value;
}

uint16_t Chi8P::Memory::getpc() {
  return this->ProgramCounter;
}

uint16_t Chi8P::Memory::step() {
  if (!this->valid(ProgramCounter))
    return 0;
  uint16_t value = static_cast<uint16_t>(
    (this->read(ProgramCounter) << 8) | this->read(ProgramCounter + 1));
  ProgramCounter += 2;
  return value;
}

void Chi8P::Memory::jump(uint16_t address) {
  this->ProgramCounter = address;
}
