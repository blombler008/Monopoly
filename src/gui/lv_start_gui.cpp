#include <components/lv_gui_containers.hpp> 

lv_obj_t* textInput_start_gui; 

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
  
void lv_start_loop(void) {
    // lv_obj_set_style_bg_color(lv_screen_active(), LVBG_COLOR, LV_PART_MAIN);
    // lv_obj_set_style_text_color(lv_screen_active(), LVFG_COLOR, LV_PART_MAIN);  
    // TimerHandle_t lvglTickTimer = xTimerCreate("lvglTick", pdMS_TO_TICKS(5), pdTRUE, NULL, [](TimerHandle_t xTimer){
    //     lv_tick_inc(5);
    // });
    // xTimerStart(lvglTickTimer, 0);
}

// GUI Creation
void lv_create_start_gui_async(void*) { 
    // Create a text label aligned center on top ("Monopoly Banking")
    // Monopoly_init("/sd/");
    // lv_screen_load(welcome);
    // lv_obj_t* src = lv_obj_assert_null(lv_screen_active());
    // lv_create_title_underline(src, "Monopoly Banking ₩");
     
    // lv_obj_add_event_cb(src, screen_event_cb, LV_EVENT_CLICKED, NULL);
    
    // Create a text input field
    // textInput_start_gui = lv_textarea_create(src);
    // lv_obj_align(textInput_start_gui, LV_ALIGN_CENTER, 0, -50); 
    // lv_textarea_set_one_line(textInput_start_gui, true);
    // lv_textarea_set_placeholder_text(textInput_start_gui, "Betrag eingeben");
    // lv_textarea_set_max_length(textInput_start_gui, 20);
    // lv_group_add_obj(getKeypadGroup(), textInput_start_gui);
    // lv_obj_add_event_cb(textInput_start_gui, text_input_event_cb, LV_EVENT_ALL, NULL);

    // Create a Settings Button


    // lv_obj_t * btn_settings = lv_button_create(src);
    // lv_obj_add_event_cb(btn_settings, [](lv_event_t * event) {
    //     log_i("Switching to Settings GUI");
    //     lv_screen_switch(SETTINGS_GUI, NULL);
    // }, LV_EVENT_CLICKED, NULL);
    // lv_obj_align(btn_settings, LV_ALIGN_CENTER, 0, 50);
    // lv_obj_remove_flag(btn_settings, LV_OBJ_FLAG_PRESS_LOCK);

    // lv_obj_t * btn_settings_label = lv_label_create(btn_settings);
    // lv_label_set_text(btn_settings_label, "Einstellungen");
    // lv_obj_center(btn_settings_label);
  
    // // Create a Button (btn2) that plays a sound 
    // lv_obj_t * btn2 = lv_button_create(src);
    // lv_obj_add_event_cb(btn2, [](lv_event_t * event) {
    //     log_i("Playing sound");
    //     audio_playback_start("Haut.mp3");
    // }, LV_EVENT_CLICKED, NULL);    
    // lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 100);
    // lv_obj_remove_flag(btn2, LV_OBJ_FLAG_PRESS_LOCK);

    // lv_obj_t * btn2_settings_label = lv_label_create(btn2);
    // lv_label_set_text(btn2_settings_label, "Sound abspielen");
    // lv_obj_center(btn2_settings_label);


}

void lv_create_start_gui(void) {
    time_gui_creation_async(lv_create_start_gui_async, NULL);
    // lv_async_call(lv_create_start_gui_async, NULL);
}