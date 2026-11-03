#pragma once
#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "event_types.hpp"
#include "event_ids.hpp"

using EventHandler = void (*)(EventId id, const void* context);
using EventHandlerId = uint16_t;

constexpr EventHandlerId INVALID_HANDLER_ID = 0xFFFF;
constexpr uint16_t MAX_SUBSCRIPTIONS = 64;

void event_manager_init();

EventHandlerId subscribe(EventId id, EventHandler handler);

void fire_event(EventId id, const void* context = nullptr);

bool unsubscribe(EventHandlerId handlerId);

#endif