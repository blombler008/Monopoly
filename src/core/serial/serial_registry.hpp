#pragma once
#ifndef SERIAL_REGISTRY_HPP
#define SERIAL_REGISTRY_HPP

#include "serial_command.hpp"
#include "serial_commands_esp.hpp"
#include "serial_commands_event.hpp"
#include "serial_commands_sd.hpp"

void serial_register_command(const SerialCommand* command);
void serial_register_builtin_commands();

bool serial_execute_command(const char* root, const char* args);
bool serial_dump_help(const char* command);

#endif