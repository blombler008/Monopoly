#include <components/lv_gui_containers.hpp> 

lv_obj_t* textInput_start_gui; 

// Helper Functions
void switch_to_add_card_gui() {
    log_i("Switching to Card Add GUI");
    const char* card_name_start_gui = lv_textarea_get_text(textInput_start_gui);
    lv_screen_switch(ADD_CARD_GUI, (void*)card_name_start_gui);
}

// Event Callbacks
static void btn1_event_cb(lv_event_t * event) {
    switch_to_add_card_gui();
}

static void text_input_event_cb(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_FOCUSED || code == LV_EVENT_READY || code == LV_EVENT_DEFOCUSED) {
        show_keyboard_on_click(event, textInput_start_gui);
    }
}

static void screen_event_cb(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_CLICKED) {
        remove_keyboard_and_clear_focus();
    }
}

// Main Loop
void displayloop(void*) { 
    while (1) {
        delay(20);
        lv_task_handler();  // let the GUI do its work
        lv_timer_handler();
        // lv_tick_inc(lv_timer_handler());     // tell LVGL how much time has passed
    }
}

void lv_start_loop(void) {
    lv_obj_set_style_bg_color(lv_screen_active(), LVBG_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(lv_screen_active(), LVFG_COLOR, LV_PART_MAIN);
    xTaskCreatePinnedToCore(displayloop, "display", 20000, NULL, 2, NULL, 0); // Increase stack size to 20000
}

// GUI Creation
void lv_create_start_gui_async(void*) { 
    // Create a text label aligned center on top ("Monopoly Banking")
    lv_obj_t* src = lv_obj_assert_null(lv_screen_active());
    lv_create_title_underline(src, "Monopoly Banking ₩");
     
    lv_obj_add_event_cb(src, screen_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Create a text input field
    // textInput_start_gui = lv_textarea_create(src);
    // lv_obj_align(textInput_start_gui, LV_ALIGN_CENTER, 0, -50); 
    // lv_textarea_set_one_line(textInput_start_gui, true);
    // lv_textarea_set_placeholder_text(textInput_start_gui, "Betrag eingeben");
    // lv_textarea_set_max_length(textInput_start_gui, 20);
    // lv_group_add_obj(getKeypadGroup(), textInput_start_gui);
    // lv_obj_add_event_cb(textInput_start_gui, text_input_event_cb, LV_EVENT_ALL, NULL);

    // Create a Settings Button
    lv_obj_t * btn_settings = lv_button_create(src);
    lv_obj_add_event_cb(btn_settings, [](lv_event_t * event) {
        log_i("Switching to Settings GUI");
        lv_screen_switch(SETTINGS_GUI, NULL);
    }, LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn_settings, LV_ALIGN_CENTER, 0, 50);
    lv_obj_remove_flag(btn_settings, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn_settings_label = lv_label_create(btn_settings);
    lv_label_set_text(btn_settings_label, "Einstellungen");
    lv_obj_center(btn_settings_label);

    // Create a Button (btn1)
    lv_obj_t * btn1 = lv_button_create(src);
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    lv_obj_t * btn_label = lv_label_create(btn1);
    lv_label_set_text(btn_label, "Karte hinzufügen");
    lv_obj_center(btn_label);
}

void lv_create_start_gui(void) {
    time_gui_creation_async(lv_create_start_gui_async, NULL);
}