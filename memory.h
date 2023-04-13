#pragma once
#include <cstddef>

// typedef unsigned char OpCode[4];

class Memory {
    union { // 4096 (4KB)
        unsigned char _Memory[0x1000] {};
        struct {
            unsigned char FontSet[0x80];
            unsigned char FrameBuffer[0x100]; // 64*32/8
            unsigned char Reserved[0x60]; // Not used
            unsigned char Stack[8];
            unsigned char V[16];
            unsigned char DelayTimer;
            unsigned char SoundTimer;
            unsigned char StackPointer;
            unsigned short IndexRegister;
            unsigned short ProgramCounter;
            // Here Be Dragons
            unsigned char ProgramSpace[0];
        };
    };
    static_assert(offsetof(Memory, ProgramSpace) == 0x200,
        "Program space is not at the correct address");
public:
    unsigned char read(unsigned short);
    void write(unsigned short, unsigned char);

    const OpCode& nextOp();
};