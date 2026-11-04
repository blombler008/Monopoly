#pragma once
#ifndef EVENT_IDS_HPP
#define EVENT_IDS_HPP

#include <stdint.h>

#define EVENT_LIST(X) \
    X(UiWelcome) \
    X(UiAddPlayer) \
    X(UiRemovePlayer) \
    X(UiStartGame) \
    X(RfidCardDetected) \
    X(RfidCardRemoved) \
    X(PlayerAdded) \
    X(PlayerRemoved) \
    X(GameStarted) \
    X(GameEnded) \
    X(SDCardDetected) \
    X(SDCardRemoved) \
    X(AudioBeepRequested) \
    X(AudioVolumeSet) \
    X(AudioPlayback) \
    X(AudioMute)
;
enum class EventId : uint16_t {
#define GENERATE_ENUM(name) name,
    EVENT_LIST(GENERATE_ENUM)
#undef GENERATE_ENUM
};
  
#endif