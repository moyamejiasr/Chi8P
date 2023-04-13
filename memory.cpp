#include "memory.h"

unsigned char Memory::read(unsigned short address) {
    return this->_Memory[address];
}

void Memory::write(unsigned short address, unsigned char value) {
    this->_Memory[address] = value;
}