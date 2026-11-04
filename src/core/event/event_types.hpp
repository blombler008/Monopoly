#pragma once
#ifndef EVENT_TYPES_HPP
#define EVENT_TYPES_HPP

#include <stdint.h>


struct StartGameContext {
    uint8_t playerCount;
    uint32_t startMoney;
};

struct RfidCardContext {
    uint32_t uid;
};

enum class AudioWaveform : uint8_t {
    Sine,
    Square,
    Triangle,
    Saw
};

enum class AudioBeepType : uint8_t {
    Custom,
    Single,
    Continuous,
    Fast_High_Continuous,
};

struct AudioBeepContext {
    AudioBeepType type;
    AudioWaveform shape;
    float frequency;
    uint32_t durationMs;
    float volume;
};

struct AudioVolumeContext {
    uint8_t volume;
};

struct AudioPlaybackContext {
    bool start;
    const char* path = nullptr;
};



#endif