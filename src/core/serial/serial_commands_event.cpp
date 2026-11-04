#include "serial_commands_event.hpp"

#include <string.h>
#include <Arduino.h>
#include "core/event/event_manager.hpp"
#include "serial_registry.hpp"

static const SerialCommand EVENT_COMMAND = {
    "events",
    "Event manager commands",
    serial_event_execute,
    serial_event_help,
};

void register_event_commands() {
    serial_register_command(&EVENT_COMMAND);    
}

static void serial_event_help() {
    log_i("");
    log_i("EVENT commands:");
    log_i("");
    log_i("events dump");
    log_i("");
}

bool serial_event_execute(const char* command) {
    if (strcmp(command, "dump") == 0)
    {
        dump_events();
        return true;
    }

    return false;
}