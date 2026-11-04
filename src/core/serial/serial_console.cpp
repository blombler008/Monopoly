#include "serial_console.hpp"
#include <Arduino.h>
#include <string.h>

static char s_buffer[128];
static size_t s_bufferPos = 0;

#include "serial_commands_sd.hpp"
#include "serial_commands_event.hpp"
#include "serial_commands_esp.hpp"
static void cmd_help(); 
 
void serial_console_setup() {
    memset(
        s_buffer,
        0,
        sizeof(s_buffer));
    log_i("Setup done, type \"help\" for help.");
}

static void dispatch_command(const char* command) {
    char root[32];
    char args[96];

    root[0] = '\0';
    args[0] = '\0';

    sscanf(
        command,
        "%31s %95[^\n]",
        root,
        args);

    if (strcmp(root, "sd") == 0)
    {
        serial_sd_execute(args);
        return;
    }

    if (strcmp(root, "esp") == 0)
    {
        serial_esp_execute(args);
        return;
    }

    if (strcmp(root, "events") == 0)
    {
        serial_event_execute(args);
        return;
    }
 
    log_w("Unknown command");
}


void serial_console_update()
{
    while (Serial.available())     {
        char c = Serial.read();

        if (c == '\r')
            continue;

        if (c != '\n')
        {
            if (s_bufferPos <
                sizeof(s_buffer) - 1)
            {
                s_buffer[s_bufferPos++] = c;
            }

            continue;
        }

        s_buffer[s_bufferPos] = '\0';

        if (s_bufferPos > 0)
        {
            dispatch_command(
                s_buffer);
        }

        s_bufferPos = 0;
        s_buffer[0] = '\0';
    }
}

static void cmd_help() {
    log_i("");
    log_i("Commands:");
    log_i("  help");
    log_i("  events dump");
    log_i("  sd reload");
    log_i("");
}

static void cmd_events_dump() {
    dump_events();
}

static void cmd_sd_reload() {
    log_i("Reloading SD...");

    fire_event(EventId::SDCardRemoved);

    vTaskDelay(pdMS_TO_TICKS(20));

    fire_event(EventId::SDCardDetected);
}