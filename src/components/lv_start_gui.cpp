#include "lv_gui_containers.hpp" 

lv_obj_t* slider_label;
lv_obj_t* slider;
lv_obj_t* sw_label;
lv_obj_t * textInput;
 
static void btn1_event_cb(lv_event_t * event) {
    
    log_i("Switching to Card Add GUI");
    const char* card_name = lv_textarea_get_text(textInput);
    lv_screen_switch(ADD_CARD_GUI, (void*)card_name);
}

static void text_input_event_cb(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(lv_keyboard_create(lv_screen_active()), textInput); 
    }  
}

void displayloop(void*) { 
    while (1) {
        lv_task_handler();  // let the GUI do its work
        lv_timer_handler();
        lv_tick_inc(lv_timer_handler());     // tell LVGL how much time has passed
    }
}

void lv_start_loop(void) {
    lv_obj_set_style_bg_color(lv_screen_active(), LVBG_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(lv_screen_active(), LVFG_COLOR, LV_PART_MAIN);
    xTaskCreatePinnedToCore(displayloop, "display", 20000, NULL, 2, NULL, 0); // Increase stack size to 20000
}

void lv_create_start_gui(void) { 
    // Create a text label aligned center on top ("Monopoly Banking")
    lv_obj_t * text_label = lv_label_create(lv_screen_active());
    lv_label_set_text(text_label, "Monopoly Banking ₩"); 
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);
    
    // Create a text input field
    textInput = lv_textarea_create(lv_screen_active());
    lv_obj_align(textInput, LV_ALIGN_CENTER, 0, -50); 
    lv_textarea_set_one_line(textInput, true);
    lv_textarea_set_placeholder_text(textInput, "Betrag eingeben");
    lv_textarea_set_max_length(textInput, 20);
    lv_group_add_obj(getKeypadGroup(), textInput);
    lv_obj_add_event_cb(textInput, text_input_event_cb, LV_EVENT_ALL, NULL);

    // Create a Button (btn1)
    lv_obj_t * btn1 = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_t * btn_label = lv_label_create(btn1);
    lv_label_set_text(btn_label, "Karte hinzufügen");
    lv_obj_center(btn_label);
}

