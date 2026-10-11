#include <components/lv_gui_containers.hpp> 
#include <Monopoly.h>
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

void lvglTask(void *pvParameters) {  
    while (true) { 
        vTaskDelay(pdMS_TO_TICKS(lv_timer_handler())); 
    }
}
void lv_start_loop(void) {
    // lv_obj_set_style_bg_color(lv_screen_active(), LVBG_COLOR, LV_PART_MAIN);
    // lv_obj_set_style_text_color(lv_screen_active(), LVFG_COLOR, LV_PART_MAIN);  
    // xTaskCreatePinnedToCore(
    //     lvglTask,
    //     "lvglTick",
    //     8192,
    //     NULL,
    //     1,
    //     NULL,
    //     1   // Core 1 empfohlen
    // );
    // TimerHandle_t lvglTickTimer = xTimerCreate("lvglTick", pdMS_TO_TICKS(5), pdTRUE, NULL, [](TimerHandle_t xTimer){
    //     lv_tick_inc(5);
    // });
    // xTimerStart(lvglTickTimer, 0);
}



// GUI Creation
void lv_create_start_gui_async(void*) { 
    // Create a text label aligned center on top ("Monopoly Banking")
    // Link Arduino SD to LVGL
    Monopoly_init("S:/assets/");
    // Supported Languages: English ( "en" ), Deutsch ( "de" )  
    // default = en
    // lv_result_t project_result = lv_xml_register_component_from_file("S:/assets/project.xml");
    // lv_xml_register_component_from_file("S:/assets/globals.xml");
    // lv_result_t translation_result = lv_xml_register_translation_from_file("S:/assets/translation.xml"); 

    // log_i("Translation registration result: %d", translation_result);
 

    welcome = welcome_create(); 
    settings = settings_create();
    game_start = game_start_create();
    
    lv_screen_load(welcome);

    lv_translation_set_language("de"); 

 
}

void lv_create_start_gui(void) {
    time_gui_creation_async(lv_create_start_gui_async, NULL);
    // lv_async_call(lv_create_start_gui_async, NULL);
}