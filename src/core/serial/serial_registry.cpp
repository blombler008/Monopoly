#include "serial_registry.hpp"

#include <Arduino.h>

static const SerialCommand* s_commands[32];
static uint8_t s_commandCount = 0;

void serial_register_builtin_commands() {
    register_sd_commands();
    register_event_commands();
    register_esp_commands();
    register_audio_commands();
}

void serial_register_command(const SerialCommand* command) {
    if (s_commandCount >= 32) return;

    s_commands[s_commandCount++] = command;
}

bool serial_execute_command(const char* root, const char* args) {
    log_i("> %s %s", root, args);
    if (strcmp("help", root) == 0) return serial_dump_help(args);

    for (uint8_t i = 0;i < s_commandCount;i++) {
        const SerialCommand* cmd = s_commands[i];  

        if (strcmp(cmd->command, root) != 0) continue;
        
        return cmd->handler(args);
    }

    return false;
}

bool serial_dump_help(const char* command) {

   if(command == nullptr || command[0] == '\0') {
        log_i("");
        log_i("Available commands:");
        log_i("");

        for(uint8_t i = 0;i < s_commandCount;i++) {
            log_i(
                "%-10s - %s",
                s_commands[i]->command,
                s_commands[i]->description);
        }

        log_i("");
        log_i("Use: help <command>");
        log_i("");

        return true;
    }

    for(uint8_t i = 0;i < s_commandCount;i++) {
        const SerialCommand* cmd = s_commands[i];

        if(strcmp(cmd->command, command) != 0) continue;
        if(cmd->help != nullptr) cmd->help();

        return true;
    }

    return false;
}