#pragma once
#ifndef SERIAL_COMMANDS_SD_HPP
#define SERIAL_COMMANDS_SD_HPP

#include "serial_command.hpp"

void register_sd_commands();
static void serial_sd_help();
bool serial_sd_execute(const char* command);

#endif