// Copyright 2023 Ricardo Moya Mejias
#pragma once
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#define _DEBUG

#define CLR_R "\x1b[31m"
#define CLR_G "\x1b[32m"
#define CLR_Y "\x1b[33m"
#define CLR_B "\x1b[34m"
#define CLR_M "\x1b[35m"
#define CLR_W "\x1b[0m"
#define STR(X) (std::stringstream() << X).str()
#define HEX(X) std::hex << std::setw(4) << std::setfill('0') << X
// IO macros
#if defined _DEBUG
#define CLOG(X) std::cout << "[D] " << STR(X) << std::endl
#else
#define CLOG(X)
#endif
#define COUT(X) std::cout << "[*] " << STR(X) << std::endl
#define CERR(X) std::cout << "[!] " << STR(X) << std::endl

#define SCREEN_SCALE 10
#define FRAMEBUFFER_WIDTH 64
#define FRAMEBUFFER_HEIGHT 32
#define SCREEN_WIDTH (FRAMEBUFFER_WIDTH * SCREEN_SCALE)
#define SCREEN_HEIGHT (FRAMEBUFFER_HEIGHT * SCREEN_SCALE)

#define MSG_PGTITLE     "Chi8P - Chip8 OOP Emulator"
#define MSG_WELCOME     CLR_G << MSG_PGTITLE << CLR_W
#define MSG_STARTUP     "Initializing..."
#define MSG_PROCEED     "Running main thread..."

#define MSG_DBGEXEC(X)  CLR_Y << "Executing: " << HEX(X) << CLR_W
#define MSG_DBGEOPG     CLR_B << "End of Program. Press any key to exit" << CLR_W  // NOLINT(whitespace/line_length)

#define MSG_ERROPEN     CLR_R << "Error: Could not read file." << CLR_W
#define MSG_ERRSDLI     CLR_R << "Error: Could not initialize SDL." << CLR_W
#define MSG_ERRWNDI     CLR_R << "Error: Could not create window." << CLR_W
#define MSG_ERRRNDI     CLR_R << "Error: Could not create renderer." << CLR_W
#define MSG_ERRTXDI     CLR_R << "Error: Could not create texture." << CLR_W
#define MSG_ERRNOOP(X)  CLR_R << "Warning: Operation 0x" << HEX(X) << " not supported. Skipped." << CLR_W   // NOLINT(whitespace/line_length)

namespace Chi8P {
class Window;
class Memory;
class Processor;
typedef void (Processor::*Instruction)(uint16_t);
}

// Bugfix GCC sstream implementation
// https://stackoverflow.com/questions/30065080/stdstringstream-and-the-str-method
template<typename Type>
std::stringstream& operator<<(std::stringstream& ss, const Type& type) {
    static_cast<std::ostream&>(ss) << type;
    return ss;
}

template<typename Type>
std::stringstream& operator<<(std::stringstream&& ss, const Type& type) {
    static_cast<std::ostream&>(ss) << type;
    return ss;
}
