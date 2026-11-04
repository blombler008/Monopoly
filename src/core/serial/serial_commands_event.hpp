#pragma once
#ifndef SERIAL_COMMANDS_EVENT_HPP
#define SERIAL_COMMANDS_EVENT_HPP

#include "serial_command.hpp"

void register_event_commands();
static void serial_event_help();
bool serial_event_execute(const char* command);

#endif  