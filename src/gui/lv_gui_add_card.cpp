#include <components/lv_gui_containers.hpp> 

#include <core/settings.h>

Card* card_scanned = NULL;
const char* default_card_name_add_card = "Neue Karte";
char card_name_add_card[21] = "Neue Karte";
// Function to switch text every 2 seconds
void switch_label_text(lv_timer_t * timer) {
    lv_obj_t * switch_label = (lv_obj_t *)timer->user_data;
    static bool toggle = false;
    toggle = !toggle;
    if(toggle) {
        lv_label_set_text(switch_label, "Suche nach neuen Karten");
        return;
    } 

    lv_label_set_text(switch_label, "Bitte Karte hin halten");
}

// Function to switch to Start Screen
void switch_to_start_screen(lv_event_t * e) { 
    if(card_scanned) {
        delete card_scanned; // Delete the card object if it exists
        log_i("card deleted");
    }
    card_scanned = NULL;
    log_i("Switching to Start GUI");
    lv_screen_switch(START_GUI); // Switch to the Start Screen 
}

void rfid_card_scanned(const char* uid) {
    // Handle the scanned RFID card  
    card_scanned = new Card(uid, card_name_add_card, coreSettings.economy.startCash);
    log_i("Card: UID=%s, Number=%d, Name=%s, Balance=%d", card_scanned->getUID(), card_scanned->getNumber(), card_scanned->getName(), card_scanned->getBalance());
    
    // Switch to the card found GUI
    log_i("Switching to Card Found GUI");
    lv_screen_switch(CARD_FOUND_GUI, card_scanned);
    card_scanned = NULL;
    strcpy(card_name_add_card, default_card_name_add_card);
}

void lv_create_add_card_gui_async(void* param) {
    // const char* name = (const char*)param;
    // if(strlen(name) > 1 && name[0] != '\0') {
    //     log_i("new card name: %s", name); 
    //     strcpy(card_name_add_card, name); 
    // }

    lv_obj_t* src = lv_obj_assert_null(lv_screen_active());

    // Create a text label aligned center on top ("Adding Card")
    lv_create_title_underline(src, "Karte hinzufügen");

    // Create a Button (Cancel)
    lv_obj_t * btn_cancel = lv_obj_assert_null(lv_button_create(src));
    lv_obj_align(btn_cancel, LV_ALIGN_CENTER, 0, 90);
    lv_obj_t * btn_label;

    btn_label = lv_obj_assert_null(lv_label_create(btn_cancel));
    lv_label_set_text(btn_label, "Abbrechen");
    lv_obj_center(btn_label);

    // Add event handler to Cancel button
    lv_obj_add_event_cb(btn_cancel, switch_to_start_screen, LV_EVENT_CLICKED, NULL);

    // Create a text label for switching messages
    lv_obj_t * switch_label = lv_obj_assert_null(lv_label_create(src));
    lv_label_set_text(switch_label, "Suche nach neuen Karten");
    lv_obj_set_style_text_align(switch_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(switch_label, LV_ALIGN_CENTER, 0, -20); // Align to center, 20 pixels above center/spinner

    // Create a spinning animated searching symbol
    lv_obj_t * spinner = lv_obj_assert_null(lv_spinner_create(src));
    lv_obj_set_size(spinner, 35, 35); // Reduced size by 30%
    lv_obj_set_style_arc_width(spinner, lv_obj_get_style_arc_width(spinner, LV_PART_MAIN) / 2, LV_PART_MAIN); // Reduced walls by 50%
    lv_obj_set_style_arc_width(spinner, lv_obj_get_style_arc_width(spinner, LV_PART_INDICATOR) / 2, LV_PART_INDICATOR); // Reduced spinning part walls by 50%
    lv_obj_set_style_arc_color(spinner, LVBG_COLOR, LV_PART_MAIN); // Set main part color to LVBG_COLOR
    lv_obj_set_style_arc_color(spinner, LVFG_COLOR, LV_PART_INDICATOR); // Set indicator part color to LVFG_COLOR
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 20); // Align to center, 20 pixels below center/spinner

    // Timer to switch text every 2 seconds
    lv_timer_t* timer = getTimer();
    timer = lv_timer_create(switch_label_text, 2000, switch_label);
    setTimer(timer);

    // Set the RFID callback to handle new card scans
    set_rfid_callback(rfid_card_scanned);
}

void lv_create_add_card_gui(const char* name) {
    time_gui_creation_async(lv_create_add_card_gui_async, (void*)name);
}