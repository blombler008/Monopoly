#pragma once
#ifndef SERIAL_COMMANDS_ESP_HPP
#define SERIAL_COMMANDS_ESP_HPP

#include "serial_command.hpp"

void register_esp_commands();

static void serial_esp_help();
bool serial_esp_execute(const char* command);

#endif