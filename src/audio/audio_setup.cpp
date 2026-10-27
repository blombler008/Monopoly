#include "audio.hpp"

Audio audio;

bool isPlaying = false;
bool killATask = false;

void audioloop(void *) {    
    while (1) { 
        if(!isPlaying) {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        audio.loop();

        vTaskDelay(1);

        if(killATask) {
            vTaskDelete(NULL);
        }
       
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

void play_beeb_single() {
    //audio_playback_start("/audio/beep.mp3"); 
}
void play_beeb_fast() {
    audio.loop();
    //audio_playback_start("/audio/beep.mp3"); 
}
void play_beeb_high() {
    // audio.setLoop(true); 
    //audio_playback_start("/audio/beep.mp3"); 
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
    audio.setVolume(21); 
    audio.setTone(-3,-3,-12); 
    audio_mute();
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