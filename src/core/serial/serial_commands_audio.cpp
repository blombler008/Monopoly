#include "serial_commands_audio.hpp"

#include <string.h>
#include <Arduino.h>
#include <esp_system.h>
#include "serial_registry.hpp"
#include "core/event/event_manager.hpp"

static const SerialCommand AUDIO_COMMAND = {
    "audio",
    "Audio commands",
    serial_audio_execute,
    serial_audio_help,
};

void register_audio_commands() {
    serial_register_command(&AUDIO_COMMAND);
}

static void serial_audio_help() {
    log_i("");
    log_i("Audio commands:");
    log_i(""); 
    log_i("audio volume <0-21>");
    log_i("audio play <path>");
    log_i("audio test");
    log_i("audio stop");
    log_i("");
}

bool serial_audio_execute(const char* command) {
   uint32_t volume;

    if(sscanf( command, "volume %lu", &volume) == 1) {
        AudioVolumeContext ctx {
            .volume = static_cast<uint8_t>((volume > 21) ? 21 : volume)
        };

        fire_event(EventId::AudioVolumeSet, &ctx);

        return true;
    }

    if (strcmp(command, "test") == 0) {
        AudioBeepContext ctx {
            .type = AudioBeepType::Custom,
            .shape = AudioWaveform::Sine,
            .frequency = 1000.0f,
            .durationMs = 600000, 
            .volume = 1.0f,
        };

        fire_event(
            EventId::AudioBeepRequested,
            &ctx);

        return true;
    }
    if(strcmp(command, "stop") == 0) {
        AudioPlaybackContext ctx {
            .start = false,
            .path = nullptr
        };

        fire_event(EventId::AudioPlayback, &ctx);

        return true;
    }

    char path[256];

    if(sscanf(command, "play %255[^\n]", path) == 1) {
        AudioPlaybackContext ctx {
            .start = true,
            .path = path
        };

        fire_event(EventId::AudioPlayback, &ctx);

        return true;
    }

    return false;
}
