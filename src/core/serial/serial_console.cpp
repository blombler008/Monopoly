#include "serial_console.hpp"
#include <Arduino.h>
#include <string.h>

static char s_buffer[128];
static size_t s_bufferPos = 0;


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
     
    bool res = serial_execute_command(root, args); 
    if(!res)  
        log_w("Unknown command: %s", command);
}

void serial_console_update() {
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\r')
            continue;

        if (c == '\b' || c == 0x7F) {
            if (s_bufferPos > 0) {
                --s_bufferPos;
                s_buffer[s_bufferPos] = '\0';
            }

            continue;
        }

        if (c == 0x1B) {
            while (Serial.available()) {
                char next = Serial.read();

                if ((next >= 'A' && next <= 'Z') ||
                    (next >= 'a' && next <= 'z'))
                {
                    break;
                }
            }

            continue;
        }

        if (c == '\n') {
            s_buffer[s_bufferPos] = '\0';

            char* command = s_buffer;

            while (*command == ' ')
                ++command;

            char* end = command + strlen(command);

            while (end > command &&
                   *(end - 1) == ' ')
            {
                --end;
            }

            *end = '\0';

            if (*command != '\0')
                dispatch_command(command);

            s_bufferPos = 0;
            memset(
                s_buffer,
                0,
                sizeof(s_buffer));

            continue;
        }

        if ((uint8_t)c < 32)
            continue;

        if (s_bufferPos >= sizeof(s_buffer) - 1)
            continue;

        s_buffer[s_bufferPos++] = c;
        s_buffer[s_bufferPos] = '\0';
    }
}
