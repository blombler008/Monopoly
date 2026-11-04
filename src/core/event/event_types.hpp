#pragma once
#ifndef EVENT_TYPES_HPP
#define EVENT_TYPES_HPP

#include <stdint.h>

enum class AudioWaveform : uint8_t {
    Sine,
    Square,
    Triangle,
    Saw
};
struct StartGameContext {
    uint8_t playerCount;
    uint32_t startMoney;
};

struct RfidCardContext {
    uint32_t uid;
};

struct AudioBeepContext {
    AudioWaveform shape;
    float frequency;
    uint32_t durationMs;
};

#endif