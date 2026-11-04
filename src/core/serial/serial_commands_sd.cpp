#include "serial_commands_sd.hpp"

#include <string.h>
#include <Arduino.h>
#include "core/event/event_ids.hpp"
#include "core/event/event_manager.hpp"
#include "serial_registry.hpp"

static const SerialCommand SD_COMMAND = {
    "sd",
    "SD management",
    serial_sd_execute,
    serial_sd_help
};

void register_sd_commands() {
    serial_register_command(&SD_COMMAND);
}

static void serial_sd_help() {
    log_i("");
    log_i("SD commands:");
    log_i("");
    log_i("sd detect");
    log_i("sd remove");
    log_i("sd reload");
    log_i("");
}

bool serial_sd_execute(const char* command) {
    if (strcmp(command, "reload") == 0){
        fire_event(EventId::SDCardRemoved);

        vTaskDelay(pdMS_TO_TICKS(20));

        fire_event(EventId::SDCardDetected);

        return true;
    }

    if (strcmp(command, "detect") == 0) {
        fire_event(EventId::SDCardDetected);
        return true;
    }

    if (strcmp(command, "remove") == 0) {
        fire_event(EventId::SDCardRemoved);
        return true;
    }

    return false;
}