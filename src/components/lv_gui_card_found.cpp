#include "lv_gui_containers.hpp"

// Function to save card name on Enter
void save_card_name_on_enter(lv_event_t* e) {
    lv_keyboard_mode_t mode = lv_keyboard_get_mode(e->target);
    if (mode == LV_KEYBOARD_MODE_TEXT_LOWER || mode == LV_KEYBOARD_MODE_TEXT_UPPER) {
        const char* card_name = lv_textarea_get_text(lv_keyboard_get_textarea(e->target));
        Card* card = static_cast<Card*>(lv_event_get_user_data(e));
        card->setName(card_name);
        save_card(card);
        lv_obj_add_flag(e->target, LV_OBJ_FLAG_HIDDEN);
    }
}

// Function to show keyboard when text field is clicked
void show_keyboard_on_click(lv_event_t* e) {
    lv_obj_t* kb = (lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
}

void lv_create_card_found_gui(Card* card) {
    lv_obj_t* scr = lv_scr_act();
    
    // Create title
    lv_obj_t* title = lv_label_create(scr);
    lv_label_set_text(title, "Karte gefunden");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Create text field
    lv_obj_t* text_field = lv_textarea_create(scr);
    lv_textarea_set_placeholder_text(text_field, "Name der Karte eingeben");
    lv_obj_align(text_field, LV_ALIGN_CENTER, 0, 0);

    // Create keyboard
    lv_obj_t* keyboard = lv_keyboard_create(scr);
    lv_keyboard_set_textarea(keyboard, text_field);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);

    // Show keyboard when text field is clicked
    lv_obj_add_event_cb(text_field, show_keyboard_on_click, LV_EVENT_CLICKED, keyboard);

    // Save card name on Enter
    lv_obj_add_event_cb(keyboard, save_card_name_on_enter, LV_EVENT_READY, card);
}