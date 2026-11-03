#pragma once
#ifndef EVENT_IDS_HPP
#define EVENT_IDS_HPP

#include <stdint.h>

enum class EventId : uint16_t {
    UiAddPlayer,
    UiRemovePlayer,
    UiStartGame,

    RfidCardDetected,
    RfidCardRemoved,

    PlayerAdded,
    PlayerRemoved,

    GameStarted,
    GameEnded,

    SDCardDetected,
    SDCardRemoved,

    AudioBeepRequested
};

#endif