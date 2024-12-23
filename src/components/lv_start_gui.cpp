#include "lv_gui_containers.hpp" 

lv_obj_t* textInput;
lv_obj_t* keyboard;

bool hasKeybord = false;
// Helper Functions
void switch_to_add_card_gui() {
    log_i("Switching to Card Add GUI");
    const char* card_name = lv_textarea_get_text(textInput);
    lv_screen_switch(ADD_CARD_GUI, (void*)card_name);
}

void remove_keyboard_and_clear_focus() {
    if(hasKeybord) { 
        lv_obj_del(keyboard);
        keyboard = nullptr;
    } 
    lv_obj_clear_state(textInput, LV_STATE_FOCUSED);
    hasKeybord = false;
}

// Event Callbacks
static void btn1_event_cb(lv_event_t * event) {
    switch_to_add_card_gui();
}

static void text_input_event_cb(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if(code == LV_EVENT_FOCUSED) {
        keyboard = lv_keyboard_create(lv_screen_active());
        lv_keyboard_set_textarea(keyboard, textInput); 
        hasKeybord = true;
        return;
    }

    if(code == LV_EVENT_DEFOCUSED) {
        remove_keyboard_and_clear_focus();
        return;
    }

    if(code == LV_EVENT_READY) {
        remove_keyboard_and_clear_focus();
        switch_to_add_card_gui();
        return;
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
    lv_obj_t* src = lv_obj_create_assert_null(lv_screen_active());
    lv_create_title_underline(src, "Monopoly Banking ₩");
    
    // lv_obj_t * text_label = lv_label_create(src);
    // lv_label_set_text(text_label, "Monopoly Banking ₩"); 
    // lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    // lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);

    // Add event handler to the screen
    lv_obj_add_event_cb(src, screen_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Create a text input field
    textInput = lv_textarea_create(src);
    lv_obj_align(textInput, LV_ALIGN_CENTER, 0, -50); 
    lv_textarea_set_one_line(textInput, true);
    lv_textarea_set_placeholder_text(textInput, "Betrag eingeben");
    lv_textarea_set_max_length(textInput, 20);
    lv_group_add_obj(getKeypadGroup(), textInput);
    lv_obj_add_event_cb(textInput, text_input_event_cb, LV_EVENT_ALL, NULL);

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

lv_obj_t* lv_obj_create_assert_null(lv_obj_t* obj) {
    assert(obj != NULL); 
    return obj;
}

void lv_create_title_underline(lv_obj_t* parent, const char* title) {
    lv_obj_t* pnt = lv_obj_create_assert_null(parent);


    // Create a text label aligned center on top ("Monopoly Banking")
    lv_obj_t * title_label = lv_obj_create_assert_null(lv_label_create(pnt));
    lv_label_set_text(title_label, title); 
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    // Create a horizontal line after the title
    lv_obj_t * line = lv_obj_create_assert_null(lv_line_create(pnt));
    static lv_point_precise_t line_points[] = { {0, 0}, {240, 0} };
    lv_line_set_points(line, line_points, 2);
    lv_obj_align_to(line, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5); 
}