#pragma once
#include <string>
#include <sstream>
#include <iostream>
#define _DEBUG

#define CLR_R "\x1b[31m"
#define CLR_G "\x1b[32m"
#define CLR_Y "\x1b[33m"
#define CLR_B "\x1b[34m"
#define CLR_M "\x1b[35m"
#define CLR_W "\x1b[0m"
#define STR(X) (std::stringstream() << X).str()
// IO macros
#if defined _DEBUG
#define CLOG(X) std::cout << "[D] " << STR(X) << std::endl
#else
#define CLOG(X)
#endif
#define COUT(X) std::cout << "[*] " << STR(X) << std::endl
#define CERR(X) std::cout << "[!] " << STR(X) << std::endl

#define MSG_WELCOME CLR_G << "Chi8P - Emulator" << CLR_W
#define MSG_STARTUP "Initializing..."
#define MSG_PROCEED "Running main thread..."

#define MSG_DBGEXEC CLR_Y << "Executing operation: " << CLR_W

#define MSG_ERROPEN CLR_R << "Error: Could not read file." << CLR_W
#define MSG_ERRNOOP CLR_R << "Warning: Operation 0 not supported. Skipped." << CLR_W
namespace Chi8P {
  class Memory;
  class Processor;
  typedef void (*Instruction)(Memory&, unsigned short);
}

// Bugfix GCC sstream implementation
// https://stackoverflow.com/questions/30065080/stdstringstream-and-the-str-method
template<typename Type>
std::stringstream& operator<<(std::stringstream& ss, const Type& type)
{
    static_cast<std::ostream&>(ss) << type;
    return ss;
}

template<typename Type>
std::stringstream& operator<<(std::stringstream&& ss, const Type& type)
{
    static_cast<std::ostream&>(ss) << type;
    return ss;
}