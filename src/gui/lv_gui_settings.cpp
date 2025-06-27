#include <components/lv_gui_containers.hpp> 


    
  
// Event Callbacks
void card_add_button_cb(lv_event_t * event) {
    log_i("Card Add Button clicked. Switching to Add Card GUI");
    lv_screen_switch(ADD_CARD_GUI);
}



void btn1_event_cb(lv_event_t * e) {
    // Get the button object
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);
    
    // Get the user data (if any)
    void * user_data = lv_event_get_user_data(e);
    
    // Log the button click event
    log_i("Settings Saved clicked, user data: %p", user_data);
    // TODO: Add function-ality for the button click event

    // Here you can add more functionality, like opening a settings menu or changing settings
    // Quick text showing the button was clicked onscreen like a toast
    // Toest should have a grey round background, white text, and fade out after 1 second
    
    lv_obj_t * toastContainer = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(toastContainer, lv_color_hex(0x080808), 0); // Hintergrundfarbe
    lv_obj_set_style_radius(toastContainer, 10, 0);                        // Abgerundet
    lv_obj_set_style_pad_all(toastContainer, 10, 0);                       // Padding
    lv_obj_set_style_text_align(toastContainer, LV_TEXT_ALIGN_CENTER, 0);  // wirkt auch auf Label
    lv_obj_set_size(toastContainer, LV_SIZE_CONTENT, LV_SIZE_CONTENT);  // Größe an Inhalt anpassen 
    lv_obj_align(toastContainer, LV_ALIGN_BOTTOM_MID, 0, -20); 
    
    lv_obj_t * toast = lv_label_create(toastContainer);
    lv_label_set_text(toast, "Einstellungen Gespeichert!");
    lv_obj_set_style_text_align(toast, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(toast, lv_color_hex(0xFFFFFF), 0);
        
    lv_obj_fade_out(toastContainer, 1000, 1000); // Fade out after 1 second
    lv_obj_add_event_cb(toastContainer, [](lv_event_t * e) {
        lv_obj_del((lv_obj_t*)lv_event_get_target(e)); // Delete the toast after fading out
    }, LV_EVENT_DELETE, NULL);


}

void lv_create_settings_gui_async(void* param) {
    lv_obj_t* src = lv_obj_assert_null(lv_screen_active());
    // Create a text label aligned center on top ("Settings")
    lv_create_title_underline(src, "Einstellungen");
  


    // Create a Button for adding Cards (card_add_button)
    lv_obj_t * card_add_button = lv_button_create(src);
    lv_obj_add_event_cb(card_add_button, card_add_button_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(card_add_button, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(card_add_button, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * card_add_button_label = lv_label_create(card_add_button);
    lv_label_set_text(card_add_button_label, "Karte hinzufügen");
    lv_obj_center(card_add_button_label);

    // Create a Button (btn1)
    lv_obj_t * btn1 = lv_button_create(lv_screen_active());
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -50);
    lv_obj_t * btn_label;

    btn_label = lv_label_create(btn1);
    lv_label_set_text(btn_label, "Speichern");
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_center(btn_label);
}

void lv_create_settings_gui(void) {
    time_gui_creation_async(lv_create_settings_gui_async, NULL);
}