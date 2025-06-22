#include "audio.hpp"

Audio audio;

bool isPlaying = false;
bool killATask = false;

void audioloop(void *) {
    while (1) {
        delayMicroseconds(100); // Allow other tasks to run, adjust as needed
        // delay(1); // To ensure the task is watchdog-safe 
        audio.loop();
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
    delay(10); 
    audio.connecttoSD(song_path);
    isPlaying = true;
}

void audio_playback_stop() {
    audio_mute();
    audio.stopSong(); 
    isPlaying = false;
}

void audio_setup() {
 
    pinMode(I2S_MUTE, OUTPUT); 
    audio_mute();

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(12); 
    audio.setTone(-3,-3,-12); 
    log_i("Audio Driver initialized");
 
}

void audio_start_loop() {
    xTaskCreatePinnedToCore(audioloop, "audio", 10000, NULL, 1, NULL, 1);
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

void audio_info(const char *info){
    log_v("info        :%s", info);
}
void audio_id3data(const char *info){  //id3 metadata
    log_v("id3data     :%s", info);
} 

void audio_eof_mp3(const char *info){  //end of file
    log_v("eof_mp3     :%s", info); 
    audio_playback_stop();
    audio_mute();
    // TODO: Listed queue
}
void audio_showstation(const char *info){
    log_v("station     :%s", info);
}
void audio_showstreamtitle(const char *info){
    log_v("streamtitle :%s", info);
}
void audio_bitrate(const char *info){
    log_v("bitrate     :%s", info);
}
void audio_commercial(const char *info){  //duration in sec
    log_v("commercial  :%s", info);
}
void audio_icyurl(const char *info){  //homepage
    log_v("icyurl      :%s", info);
}
void audio_lasthost(const char *info){  //stream URL played
    log_v("lasthost    :%s", info);
}





