#include "synth.hpp"

static SynthVoice g_voice;

static constexpr float SAMPLE_RATE = 48000.0f;
static constexpr float PI2 = 6.28318530718f;
static constexpr uint32_t FADE_SAMPLES = static_cast<uint32_t>(SAMPLE_RATE * 0.005f); // 5 ms

static float generateSample() {
    float value = 0.0f;

    switch(g_voice.shape) {
        case WaveShape::Sine: 
            value = sinf(
                g_voice.phase * PI2
            );
            break;
        

        case WaveShape::Square:
            value = (g_voice.phase < 0.5f)
                    ? 1.0f
                    : -1.0f;
            break;
        

        case WaveShape::Triangle:
            value = 2.0f *
                fabsf(
                    (2.0f * g_voice.phase) - 1.0f
                ) - 1.0f;
            break;
        

        case WaveShape::Saw:
            value = (2.0f * g_voice.phase)
                - 1.0f;
            break;
        
    }

    g_voice.phase +=
        g_voice.frequency /
        SAMPLE_RATE;

    if(g_voice.phase >= 1.0f) g_voice.phase -= 1.0f;

    return value;
}

void synth_beep(WaveShape shape, float frequency, uint32_t durationMs, float volume) {
    log_i("Synth %.0f Hz, %lu ms", frequency, durationMs);

    g_voice.active = true;

    g_voice.shape = shape;
    g_voice.frequency = frequency;
    g_voice.volume = volume;

    g_voice.phase = 0.0f;

    g_voice.remainingSamples = static_cast<uint32_t>((durationMs * SAMPLE_RATE)/ 1000.0f);
}

static void synth_fill_buffer(int32_t* buffer, uint32_t frameCount) {
    memset( buffer, 0, frameCount * 2 * sizeof(int32_t) );

    if(!g_voice.active) return;

    for(uint32_t i = 0; i < frameCount; i++) {
        if(g_voice.remainingSamples == 0) {
            g_voice.active = false;
            break;
        }

        float gain = 1.0f;

        uint32_t totalSamples = static_cast<uint32_t>(
            (g_voice.durationMs * SAMPLE_RATE)
            / 1000.0f
        );

        uint32_t playedSamples = totalSamples - g_voice.remainingSamples;

        // Fade-In
        if(playedSamples < FADE_SAMPLES) {
            gain = static_cast<float>(playedSamples) / static_cast<float>(FADE_SAMPLES);
        }

        // Fade-Out
        if(g_voice.remainingSamples < FADE_SAMPLES) {
            float fadeOut = 
                static_cast<float>( g_voice.remainingSamples)
                / static_cast<float>( FADE_SAMPLES );

            gain = std::min(gain, fadeOut);
        }
        
        int32_t sample = static_cast<int32_t>(
                generateSample()
                * gain
                * g_voice.volume
                * 8388607.0f
            );

        buffer[(i * 2) + 0] = sample;
        buffer[(i * 2) + 1] = sample;

        g_voice.remainingSamples--;
    }
}

bool synth_is_active() {
    return g_voice.active;
}

void synth_init() {
    g_voice.active = false;
    g_voice.phase = 0.0f;
    g_voice.remainingSamples = 0;
}

bool synth_write_to_i2s(i2s_chan_handle_t handle) {
    if(!g_voice.active) return false;

    constexpr uint32_t FRAME_COUNT = 256;

    int32_t buffer[FRAME_COUNT * 2];

    synth_fill_buffer(buffer, FRAME_COUNT);
 
    size_t bytesWritten = 0;

    esp_err_t err = i2s_channel_write(
            handle,
            buffer,
            sizeof(buffer),
            &bytesWritten,
            portMAX_DELAY
        );

    return err == ESP_OK;
}