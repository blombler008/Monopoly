#include <gui/lv_gui_containers.hpp> 
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

void show_toast(const char *msg) {
    lv_obj_t *toast = lv_label_create(lv_layer_top());
    lv_label_set_text(toast, msg);

    lv_obj_set_style_bg_color(toast, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(toast, LV_OPA_100, 0);
    lv_obj_set_style_text_color(toast, lv_color_white(), 0);
    lv_obj_set_style_pad_all(toast, 10, 0);

    lv_obj_align(toast, LV_ALIGN_BOTTOM_MID, 0, -10);

    // Auto‑delete nach 1 Sekunde
    lv_timer_t *t = lv_timer_create([](lv_timer_t *t){
        lv_obj_del((lv_obj_t*)t->user_data);
        lv_timer_del(t);
    }, 2333, toast);
}

void add_card_cb(lv_event_t * event) {
    enable_rfid_scan(); // Enable RFID scanning when the "Add Card" button is clicked in settings GUI
    lv_subject_set_int(&nfc_on, 1);
}


void card_found_callback(const char* uid) {
    show_toast("Karte gefunden!");
    lv_subject_set_int(&nfc_on, 0);
    // Spieler hinzufügen
    // TODO: do another screen that takes in player info 
    // add_player_with_uid(uid);     

    // UI zurück auf Settings oder weiter
    
    lv_screen_load(settings_create());
}

// GUI Creation
void lv_create_start_gui_async(void*) { 
    // Create a text label aligned center on top ("Monopoly Banking")
    // Link assets from Arduino SD to LVGL
    Monopoly_init("S:/assets/");
    // Supported Languages: English ( "en" ), Deutsch ( "de" )  
    // default = en  
    int lang = lv_subject_get_int(&language);
    lv_translation_set_language(lang == 0 ? "en" : "de"); // Set the language based on the subject value
    lv_subject_set_int(&rfid_audio_tags, get_rfid_audio_tags_enabled()); // Enable RFID audio tags by default
    lv_screen_load(welcome_create());

    set_rfid_callback(card_found_callback); // Set the RFID callback function to handle card detection events

}

void rfid_audio_tags_enabled_observer_cb(lv_observer_t * observer, lv_subject_t * subject) {
    bool enabled = lv_subject_get_int(subject) ? true : false;
    set_rfid_audio_tags_enabled(enabled);
}

void lv_create_start_gui(void) {
    time_gui_creation_async(lv_create_start_gui_async, NULL);
    // lv_async_call(lv_create_start_gui_async, NULL);
}