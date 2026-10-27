#pragma once
#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "audio.hpp"
#include <driver/i2s_std.h>

struct SynthVoice {
    bool active = false;

    WaveShape shape = WaveShape::Triangle;

    float frequency = 440.0f;
    float volume = 1.0f;

    float phase = 0.0f;

    uint32_t durationMs = 0;

    uint32_t remainingSamples = 0;
};

void synth_init();

bool synth_is_active();

void synth_beep(WaveShape shape, float frequency, uint32_t durationMs, float volume = 0.15f);

bool synth_write_to_i2s(i2s_chan_handle_t handle);
 

#endif