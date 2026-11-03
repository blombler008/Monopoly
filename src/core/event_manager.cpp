#include "event_manager.hpp"
 

struct Subscription {
    bool used;
    EventHandlerId id;
    EventId eventId;
    EventHandler handler;
};

static Subscription g_subscriptions[MAX_SUBSCRIPTIONS];
static EventHandlerId g_nextHandlerId = 0;


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
    for (const auto& subscription : g_subscriptions) {
        if (!subscription.used) continue;

        if (subscription.eventId != eventId) continue;

        if (subscription.handler == nullptr) continue;
        
        subscription.handler(eventId, context);
    }
}