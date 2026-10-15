#include "lv_setup_display.hpp"


void lv_create_title_underline(lv_obj_t* parent, const char* title) {
    lv_obj_t* pnt = lv_obj_assert_null(parent);

    // Create a text label aligned center on top ("Monopoly Banking")
    lv_obj_t * title_label = lv_obj_assert_null(lv_label_create(pnt));
    lv_label_set_text(title_label, title); 
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    // Create a horizontal line after the title
    lv_obj_t * line = lv_obj_assert_null(lv_line_create(pnt));
    static lv_point_precise_t line_points[] = { {0, 0}, {240, 0} };
    lv_line_set_points(line, line_points, 2);
    lv_obj_align_to(line, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5); 
}