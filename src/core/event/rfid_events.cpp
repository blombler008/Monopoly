#include "event_manager.hpp"
#include "event_subscriptions.hpp"
#include "rfid/rfid_module.hpp"

static EventHandlerId s_sdDetectedHandler;

void on_sd_detected(EventId, const void*) {
    rfid_load_tags_from_fs();
}

void register_rfid_events() {
    s_sdDetectedHandler = subscribe(EventId::SDCardDetected, on_sd_detected);
}