#pragma once
#ifndef SERIAL_COMMANDS_SD_HPP
#define SERIAL_COMMANDS_SD_HPP

#include "serial_command.hpp" 
void register_sd_commands();

static void serial_sd_help();
bool serial_sd_execute(const char* command);

void show_sd_info(bool more = true);

static void sd_list_directory(const char* path); 
static void sd_tree(const char* path);
static void sd_exists(const char* path);
#endif