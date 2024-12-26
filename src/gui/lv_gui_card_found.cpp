#include <components/lv_gui_containers.hpp> 

lv_obj_t* textInput_card_found; 
const char* card_name_card_found; 
Card* card; 

// Helper Functions  
void switch_to_remove_card_gui() {
    log_i("Switching to Card Remove GUI"); 
    lv_screen_switch(REMOVE_CARD_GUI, (void*)card);
}

// Function to save card name on Enter
void save_card_name_on_enter() { 
    const char* next_card_name = lv_textarea_get_text(textInput_card_found);
    
    if(strcmp(card_name_card_found, next_card_name)) return;

    card->setName(card_name_card_found);
    cardCollection.save_card(card); 
}

static void screen_event_cb(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED) remove_keyboard_and_clear_focus();

}

static void text_input_event_cb(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    show_keyboard_on_click(event, textInput_card_found); 
    if(code == LV_EVENT_READY) {
        save_card_name_on_enter();
        switch_to_remove_card_gui();
    } 

}

void lv_create_card_found_gui_async(void* card_ptr) {

    if(card_ptr == NULL) {
        log_e("Card not found");
        return;
    }

    card = static_cast<Card*>(card_ptr); 
    card_name_card_found = card->getName(); 

    // Create a screen
 
    lv_obj_t* scr = lv_obj_assert_null(lv_screen_active());
    lv_obj_add_event_cb(scr, screen_event_cb, LV_EVENT_CLICKED, NULL);

    // Create title
    lv_create_title_underline(scr, "Karte gefunden");
   
    // Create text field
    textInput_card_found = lv_obj_assert_null(lv_textarea_create(scr));
    lv_textarea_set_one_line(textInput_card_found, true);
    lv_textarea_set_text(textInput_card_found, card_name_card_found);
    lv_textarea_set_max_length(textInput_card_found, 20);
    lv_group_add_obj(getKeypadGroup(), textInput_card_found);
    lv_obj_align(textInput_card_found, LV_ALIGN_CENTER, 0, -30); 

    // Create a label for the next available card number
    lv_obj_t* next_card_label = lv_label_create(scr);
    lv_label_set_text_fmt(next_card_label, "Kartennummer: %d", cardCollection.getNextCardNumber());
    lv_obj_align(next_card_label, LV_ALIGN_CENTER, 0, -60);
  
    // Show keyboard when text field is clicked
    lv_obj_add_event_cb(textInput_card_found, text_input_event_cb, LV_EVENT_ALL, NULL); 

    // Create Save button
    lv_obj_t* save_btn = lv_obj_assert_null(lv_btn_create(scr));
    lv_obj_align(save_btn, LV_ALIGN_CENTER, -50, 50);
    lv_obj_t* save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Speichern");
    lv_obj_add_event_cb(save_btn, [](lv_event_t* event) {
        save_card_name_on_enter();
        switch_to_remove_card_gui();
    }, LV_EVENT_CLICKED, NULL);

    // Create Cancel button
    lv_obj_t* cancel_btn = lv_obj_assert_null(lv_btn_create(scr));
    lv_obj_align(cancel_btn, LV_ALIGN_CENTER, 50, 50);
    lv_obj_t* cancel_label = lv_label_create(cancel_btn);
    lv_label_set_text(cancel_label, "Abbrechen");
    lv_obj_add_event_cb(cancel_btn, [](lv_event_t* event) {
        lv_screen_switch(START_GUI, NULL);
    }, LV_EVENT_CLICKED, NULL);
}

void lv_create_card_found_gui(Card* card) {
    time_gui_creation_async(lv_create_card_found_gui_async, card);
}