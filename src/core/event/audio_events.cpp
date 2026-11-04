#include "event_manager.hpp"
#include "audio/audio.hpp"

static void on_audio_volume_set(EventId id, const void* context) {
    if(!context) return;

    const auto* event = static_cast<const AudioVolumeContext*>(context);

    int vol = event->volume; 
    if((uint8_t)vol > 21) vol = 21;

    if(vol == 0) {
        fire_event(EventId::AudioMute, event);
    }

    audio_set_volume(vol);
}

static void on_audio_playback(EventId id, const void* context) {
    if (!context) {
        audio_playback_stop();
    }
 
    const auto* event = static_cast<const AudioPlaybackContext*>(context);

    if(event->start) {
        audio_playback_start(event->path);
        return;
    }

    audio_playback_stop();
}


static void on_audio_mute(EventId id, const void* context) {
    if(!context) audio_mute();
 
    const auto* event = static_cast<const AudioVolumeContext*>(context);
    int vol = event->volume;
    if((uint8_t)vol > 21)
        audio_mute(false);
    
}
static void on_audio_beep_requested(EventId id, const void* context) {
    if (!context) return;

    const auto* event = static_cast<const AudioBeepContext*>(context);

    if(event->type == AudioBeepType::Custom)
        audio_beep(
            (WaveShape)event->shape,
            event->frequency,
            event->durationMs,
            event->volume);
}

void register_audio_events() {
    subscribe(EventId::AudioVolumeSet, on_audio_volume_set);
    subscribe(EventId::AudioPlayback, on_audio_playback); 
    subscribe(EventId::AudioMute, on_audio_mute);
    subscribe(EventId::AudioBeepRequested, on_audio_beep_requested);
}