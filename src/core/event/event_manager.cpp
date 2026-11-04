#include "event_manager.hpp"
#include <Arduino.h>

struct Subscription {
    bool used;
    EventHandlerId id;
    EventId eventId;
    EventHandler handler;
};

static Subscription g_subscriptions[MAX_SUBSCRIPTIONS];
static EventHandlerId g_nextHandlerId = 0;
static bool g_debugEvents = true;
static EventLogLevel g_eventLogLevel = EventLogLevel::Events;

static uint16_t get_subscription_count(EventId eventId) {
    uint16_t count = 0;

    for (const auto& subscription : g_subscriptions) {
        if (!subscription.used)
            continue;

        if (subscription.eventId != eventId)
            continue;

        count++;
    }

    return count;
}

const char* event_to_string(EventId id) {
#define EVENT_CASE(event) case EventId::event: return #event
    switch (id) {
        EVENT_CASE(UiAddPlayer);
        EVENT_CASE(UiRemovePlayer);
        EVENT_CASE(UiStartGame);
        EVENT_CASE(RfidCardDetected);
        EVENT_CASE(RfidCardRemoved);
        EVENT_CASE(PlayerAdded);
        EVENT_CASE(PlayerRemoved);
        EVENT_CASE(GameStarted);
        EVENT_CASE(GameEnded);
        EVENT_CASE(SDCardDetected);
        EVENT_CASE(SDCardRemoved);
        EVENT_CASE(AudioBeepRequested);
        default:
            return "Unknown";
    }
#undef EVENT_CASE
}

void event_manager_init() {
    
    for (auto& subscription : g_subscriptions) {
        subscription.used = false;
    }

    g_nextHandlerId = 0;
}


EventHandlerId subscribe(EventId eventId, EventHandler handler) {
    if (handler == nullptr) return INVALID_HANDLER_ID;

    for (auto& subscription : g_subscriptions) {
        if (subscription.used) continue;

        subscription.used = true;
        subscription.id = g_nextHandlerId++;
        subscription.eventId = eventId;
        subscription.handler = handler;

        return subscription.id;
    } 
    return INVALID_HANDLER_ID;
}

bool unsubscribe(EventHandlerId handlerId) {
    if (handlerId == INVALID_HANDLER_ID) return false;

    for (auto& subscription : g_subscriptions) {
        if (!subscription.used) continue;
        if (subscription.id != handlerId) continue;

        subscription.used = false;
        return true;
    }

    return false;
}

void fire_event(EventId eventId, const void* context) {
    
    if (g_eventLogLevel != EventLogLevel::Off) {
        log_i(
            "[EVENT] %s (%u subscriber%s)",
            event_to_string(eventId),
            get_subscription_count(eventId),
            get_subscription_count(eventId) == 1 ? "" : "s"
        );
    }

    for (const auto& subscription : g_subscriptions) {
        if (!subscription.used) continue;

        if (subscription.eventId != eventId) continue;

        if (subscription.handler == nullptr) continue;
        
        subscription.handler(eventId, context);
    }
}

void dump_events() {
    log_i("");
    log_i("===== EVENT MANAGER =====");

    for (uint16_t i = 0; i <= static_cast<uint16_t>(EventId::AudioBeepRequested); i++) {
        auto eventId = static_cast<EventId>(i);

        log_i(
            "%-20s : %u subscriber%s",
            event_to_string(eventId),
            get_subscription_count(eventId),
            get_subscription_count(eventId) == 1
                ? ""
                : "s"
        );
    }

    uint16_t used = 0;

    for (const auto& subscription : g_subscriptions) {
        if (subscription.used) used++;
    }

    log_i("");
    log_i(
        "Subscriptions: %u / %u",
        used,
        MAX_SUBSCRIPTIONS
    );

    log_i("=========================");
}