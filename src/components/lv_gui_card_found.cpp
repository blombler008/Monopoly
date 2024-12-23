#include "lv_gui_containers.hpp"

// Function to save card name on Enter
void save_card_name_on_enter(lv_event_t* e) {
    lv_obj_t* kb = (lv_obj_t*)lv_event_get_target(e);
    lv_keyboard_mode_t mode = lv_keyboard_get_mode(kb);
    if (mode == LV_KEYBOARD_MODE_TEXT_LOWER || mode == LV_KEYBOARD_MODE_TEXT_UPPER) {
        const char* card_name = lv_textarea_get_text(lv_keyboard_get_textarea(kb));
        Card* card = static_cast<Card*>(lv_event_get_user_data(e));
        card->setName(card_name);
        cardCollection.save_card(card);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

// Function to show keyboard when text field is clicked
void show_keyboard_on_click(lv_event_t* e) {
    lv_obj_t* kb = (lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
}

void lv_create_card_found_gui_async(void* card_ptr) {
    Card* card = static_cast<Card*>(card_ptr);
    lv_obj_t* scr = lv_obj_create_assert_null(lv_screen_active());
    
    // Create title
    lv_create_title_underline(scr, "Karte gefunden");
   
    // Create text field
    lv_obj_t* text_field = lv_obj_create_assert_null(lv_textarea_create(scr));
    lv_textarea_set_one_line(text_field, true);
    lv_textarea_set_placeholder_text(text_field, "Karten Name");
    lv_obj_align(text_field, LV_ALIGN_CENTER, 0, 0);
   
    // Create keyboard
    lv_obj_t* keyboard = lv_obj_create_assert_null(lv_keyboard_create(scr));
    lv_keyboard_set_textarea(keyboard, text_field);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
  
    // Show keyboard when text field is clicked
    lv_obj_add_event_cb(text_field, show_keyboard_on_click, LV_EVENT_CLICKED, keyboard);

    // Save card name on Enter
    lv_obj_add_event_cb(keyboard, save_card_name_on_enter, LV_EVENT_READY, card);
}

void lv_create_card_found_gui(Card* card) {
    time_gui_creation_async(lv_create_card_found_gui_async, card);
}