#pragma once
#ifndef SERIAL_COMMANDS_AUDIO_HPP
#define SERIAL_COMMANDS_AUDIO_HPP

#include "serial_command.hpp"

void register_audio_commands();

static void serial_audio_help();
bool serial_audio_execute(const char* command);

#endif
