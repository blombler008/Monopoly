#include "audio.hpp"
#include "synth.hpp"

Audio audio;

bool isPlaying = false;
bool killATask = false;

void audioloop(void *) {    
    while(1)
    {
        if(killATask) vTaskDelete(NULL); 

        if(isPlaying) audio.loop();
        
        else if(synth_is_active()) { 
            synth_write_to_i2s(audio.getI2SHandle());
        }
        else
            vTaskDelay(
                pdMS_TO_TICKS(10)
            );
        

        vTaskDelay(1);
    }
}

void audio_mute(bool mute) {
    if(mute) {
        digitalWrite(I2S_MUTE, AUX_MUTE);
        return;
    }
    
    digitalWrite(I2S_MUTE, AUX_UNMUTE);
}

void audio_playback_start(const char* song_path) {
    audio_playback_stop(); 
    audio_mute(false);
    delay(1); 
    audio.connecttoFS(SD, song_path);
    isPlaying = true;
}

void audio_playback_stop() {
    audio_mute();
    audio.stopSong(); 
    isPlaying = false;
}

void audio_info(Audio::msg_t m) {
    log_i("%s: %s", m.s, m.msg);

    switch (m.e) {  // make a switch statement to handle different events in case i need it.
        case Audio::evt_eof:
            audio_playback_stop();
            audio_mute(); 
            break;  
        default:
            break;
    }
}

void audio_setup() {
 
    Audio::audio_info_callback = audio_info;
    pinMode(I2S_MUTE, OUTPUT); 
    audio_mute();

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(12); 
    audio.setTone(-3,-3,-12); 
    audio_mute();
    
    synth_init();
    log_i("Audio Driver initialized");
 
}

void audio_start_loop(int core) {
    xTaskCreatePinnedToCore(audioloop, "audio", 10000, NULL, 1, NULL, core);
}

void audio_stop_loop() {
    killATask = true;
}

void audio_set_volume(uint8_t vol) {
    audio.setVolume(vol);
}

bool audio_is_playing() {
    return isPlaying;
} 

void audio_beep(WaveShape shape, float frequency, uint32_t durationMs, float volume) {
    
    if(isPlaying) return;

    audio_mute(false);
   
    synth_beep(shape, frequency, durationMs, volume*8);
    
}
 
void play_beeb_single() {
    audio_beep(
        WaveShape::Triangle,
        420,
        120
    );
}

void play_beeb_fast() {
    static uint32_t nextBeep = 0;

    uint32_t now = millis();

    if(now < nextBeep)
    {
        return;
    }

    int timeX = 50;

    nextBeep = now + timeX*2; // 120ms Ton + 120ms Pause

    audio_beep(
        WaveShape::Triangle,
        420,
        timeX
    );
}

void play_beeb_high_fast() {
    static uint32_t nextBeep = 0;

    uint32_t now = millis();

    if(now < nextBeep)
    {
        return;
    }

    int timeX = 50;

    nextBeep = now + timeX*2; // 120ms Ton + 120ms Pause

    audio_beep(
        WaveShape::Triangle,
        840,
        timeX
    ); 
}