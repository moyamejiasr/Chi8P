#include "memory.h"

unsigned char* Chi8P::Memory::data() {
    return this->ProgramSpace;
}

unsigned char Chi8P::Memory::read(unsigned short address) {
    return this->_Memory[address];
}

void Chi8P::Memory::write(unsigned short address, unsigned char value) {
    this->_Memory[address] = value;
}