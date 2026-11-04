#include "serial_commands_esp.hpp"

#include <string.h>
#include <Arduino.h>
#include <esp_system.h>
#include "serial_registry.hpp"

static const SerialCommand ESP_COMMAND = {
    "esp",
    "ESP commands",
    serial_esp_execute,
    serial_esp_help,
};

void register_esp_commands() {
    serial_register_command(&ESP_COMMAND);
}

static void serial_esp_help() {
    log_i("");
    log_i("ESP commands:");
    log_i("");
    log_i("esp reset");
    log_i("");
}

bool serial_esp_execute(const char* command) {
   if (strcmp(command, "reset") == 0){
        log_w("ESP restarting");

        esp_restart();

        return true;
    }

    return false;
}