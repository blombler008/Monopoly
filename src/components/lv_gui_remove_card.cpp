#include "lv_gui_containers.hpp"
Card* card_to_remove;
void show_start_gui() {  
    lv_screen_switch(START_GUI); // Switch to the Start Screen 
    if(card_to_remove) {
        delete card_to_remove; // Delete the card object if it exists
    }
    card_to_remove = NULL;
    
}

void on_confirm_click(lv_event_t * e) {
    log_i("karte gelöscht!");
    
    show_start_gui();
}

void on_cancel_click(lv_event_t * e) {
    log_i("karte nicht entfernt");
    show_start_gui();
}

void lv_create_remove_card_gui(Card* user_card) {
    card_to_remove = user_card; 

    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Möchtest du die Karte\nwirklich entfernen?");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * line = lv_line_create(lv_screen_active());
    static lv_point_precise_t line_points[] = { {0, 0}, {240, 0} };
    lv_line_set_points(line, line_points, 2);
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 50);

    lv_obj_t * label_uid = lv_label_create(lv_screen_active());
    lv_label_set_text(label_uid, ("UID: " + std::string(user_card->getUID())).c_str());
    lv_obj_align(label_uid, LV_ALIGN_LEFT_MID, 50, -20);

    lv_obj_t * label_card_number = lv_label_create(lv_screen_active());
    lv_label_set_text(label_card_number, ("Kartennummer: " + std::to_string(user_card->getNumber())).c_str());
    lv_obj_align(label_card_number, LV_ALIGN_LEFT_MID, 50, 0);

    lv_obj_t * label_name = lv_label_create(lv_screen_active());
    lv_label_set_text(label_name, ("Name: " + std::string(user_card->getName())).c_str());
    lv_obj_align(label_name, LV_ALIGN_LEFT_MID, 50, 20);

    lv_obj_t * btn_confirm = lv_button_create(lv_screen_active());
    lv_obj_align(btn_confirm, LV_ALIGN_BOTTOM_LEFT, 40, -30);
    lv_obj_t * label_confirm = lv_label_create(btn_confirm);
    lv_label_set_text(label_confirm, "Bestätigen");
    lv_obj_add_event_cb(btn_confirm, on_confirm_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_cancel = lv_button_create(lv_screen_active());
    lv_obj_align(btn_cancel, LV_ALIGN_BOTTOM_RIGHT, -40, -30);
    lv_obj_t * label_cancel = lv_label_create(btn_cancel);
    lv_label_set_text(label_cancel, "Abbrechen");
    lv_obj_add_event_cb(btn_cancel, on_cancel_click, LV_EVENT_CLICKED, NULL);


}
