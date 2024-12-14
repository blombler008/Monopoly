
#include "lv_gui_containers.hpp"

void lv_create_card_edit_gui(bool readOnly) {
    // Create a text label aligned center on top ("Edit Card")
    lv_obj_t * text_label = lv_label_create(lv_screen_active());
    lv_label_set_text(text_label, "Edit Card");
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);

    // Create a Button (btn1)
    lv_obj_t * btn1 = lv_button_create(lv_screen_active());
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -50);
    lv_obj_t * btn_label;

    btn_label = lv_label_create(btn1);
    lv_label_set_text(btn_label, readOnly ? "View Card" : "Save Changes");
    lv_obj_center(btn_label);
}