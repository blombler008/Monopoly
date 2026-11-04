#pragma once
#ifndef SERIAL_COMMAND_HPP
#define SERIAL_COMMAND_HPP

#include <stddef.h>

typedef bool (*SerialCommandHandler)(const char* command);

typedef void (*SerialHelpHandler)();

struct SerialCommand {
    const char* command;
    const char* description;
    SerialCommandHandler handler;
    SerialHelpHandler help;
};
 
#endif