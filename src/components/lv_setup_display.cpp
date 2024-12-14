#include "lv_setup_display.hpp"

/**
 * @struct lv_tft_espi_t
 * @brief A structure to hold a pointer to a TFT_eSPI object.
 * 
 * This structure is used to encapsulate a pointer to a TFT_eSPI object,
 * which is typically used for interfacing with TFT displays.
 * 
 * @var lv_tft_espi_t::tft
 * Pointer to a TFT_eSPI object.
 */
typedef struct {
    TFT_eSPI * tft;
} lv_tft_espi_t;

lv_tft_espi_t* displayDriver;
lv_display_t * displayInstance;
lv_indev_t * touchInputDevice;
lv_indev_t * keypadInputDevice;
lv_group_t* keypadGroup;
lv_timer_t* timer;

uint16_t colors[] = {ILI9341_PURPLE, ILI9341_RED, ILI9341_BLUE, ILI9341_GREEN, ILI9341_ORANGE, ILI9341_YELLOW, ILI9341_CYAN, ILI9341_MAGENTA, ILI9341_WHITE};
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

void lv_log(lv_log_level_t level, const char * buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}

void flush_display_buffer(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map) {
    lv_display_flush_ready(disp);
}

void keypad_read_cb(lv_indev_t * indev, lv_indev_data_t* data) {
    static uint8_t last_key = 0; // Variable to store the last key pressed
    Keypad keypad = getKeypad(); // Get the keypad object
    uint8_t key = keypad.getKey(); // Get the key pressed on the keypad
    KeyState keyState = keypad.getState(); // Get the state of the key

    if(keyState == IDLE) { // Check if the key is idle
        data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
        data->key = last_key; // Set the key to the last key pressed
        return; // Exit the function
    }


    data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
    data->key = key; // Set the key to the key pressed

    log_i("Key: %c, State: %d", key, keyState); // Log the key and state to the serial output

    if (key) {
        data->state = LV_INDEV_STATE_PRESSED; // Check if the key is pressed
    }

    last_key = key; // Set the last key to the current key
}

void touch_read_cb(lv_indev_t * indev, lv_indev_data_t* data) {
    uint16_t touchpad_x, touchpad_y; // Variables to store the touchpad coordinates
    bool touchpad_pressed = displayDriver->tft->getTouch(&touchpad_x, &touchpad_y, 300); // Get the touchpad coordinates 
    if (touchpad_pressed) { // Check if the touchpad is pressed
        data->point.y = touchpad_x; // swap x and y
        data->point.x = 240 - touchpad_y;  // swap x and y and invert x
        data->state = LV_INDEV_STATE_PRESSED; // Set the state to pressed
    } else {
        data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
    }
}

static uint32_t tick_wrapper(void) {
    return millis();
}

void lvgl_print_version() {
    char LVGL_Arduino[20]; // Create a buffer to hold the version string
    snprintf(LVGL_Arduino, sizeof(LVGL_Arduino), "LVGL V%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
    log_i("%s", LVGL_Arduino); // Log the version string to the serial output
}

void lv_setup_display(void) {
    lvgl_print_version(); // Log the version of LVGL for compatibility
    uint16_t calData[5] = TFT_CALLIBRATION_DATA;
    lv_init(); // Initialize the LVGL library
      
    lv_tick_set_cb(tick_wrapper); // Set the tick callback function for LVGL
    displayInstance = lv_tft_espi_create(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT, draw_buf, sizeof(draw_buf)); // Create a display using the TFT_eSPI library
    displayDriver = (lv_tft_espi_t*)lv_display_get_driver_data(displayInstance); // Get the display driver data
    lv_display_set_rotation(displayInstance, LV_DISPLAY_ROTATION_270); // Set the display rotation
    displayDriver->tft->setTouch(calData); // Calibrate the touch screen using the predefined calibration data


    touchInputDevice = lv_indev_create(); // Create a new input device
    lv_indev_set_type(touchInputDevice, LV_INDEV_TYPE_POINTER); // Set the input device type to pointer (touchpad)
    lv_indev_set_read_cb(touchInputDevice, touch_read_cb); // Set the read callback function for the input device

    keypadInputDevice = lv_indev_create(); // Create a new input device
    lv_indev_set_type(keypadInputDevice, LV_INDEV_TYPE_KEYPAD); // Set the input device type to keypad 
    lv_indev_set_read_cb(keypadInputDevice, keypad_read_cb); // Set the read callback function for the input device
    
    keypadGroup = lv_group_create(); // Create a new group for the keypad
    lv_indev_set_group(keypadInputDevice, keypadGroup); // Create a new group for the keypad input device
    lv_group_set_default(keypadGroup); // Set the default group for the keypad input device
}

lv_group_t* getKeypadGroup() {
    return keypadGroup;
}

lv_indev_t* getKeypadIndevDevice() {
    return keypadInputDevice;
}

lv_timer_t* getTimer() {
    return timer;
}

void delete_timer() {
    if(timer != NULL) {
        lv_timer_del(timer); // Delete the timer
        timer = NULL; // Set the timer to NULL to prevent dangling pointer
    }
}

void setTimer(lv_timer_t* timerF) {
    timer = timerF; // Set the timer
}

void lv_screen_switch(GuiScreens screen, void* user_data) {
    delete_timer(); // Delete the timer

    clear_rfid_callback(); // Clear the RFID callback

    // Clear the current screen
    lv_obj_clean(lv_scr_act());

    // Switch to the specified screen
    switch (screen) {
        case START_GUI:
            lv_create_start_gui();
            break;
        case ADD_CARD_GUI:
            if(user_data) {
                lv_create_add_card_gui((char*)user_data);
            } else {
                log_e("No user data provided for add card GUI");
            } 
            break;
        case CARD_FOUND_GUI:
            if (user_data) {
                lv_create_card_found_gui((Card*)user_data);
            } else {
                log_e("No card data provided for card found GUI");
            } 
            break;
        case CARD_EDIT_GUI:
            if (user_data) {
                bool readonly = *(bool*)user_data;
                lv_create_card_edit_gui(readonly);
            } else {
                log_e("No read-only data provided for card edit GUI");
            }
            break;
        case REMOVE_CARD_GUI:
            if (user_data) {
                lv_create_remove_card_gui((Card*)user_data);
            } else {
                log_e("No card data provided for remove card GUI");
            }
            break;
        case CARDS_GUI:
            lv_create_cards_gui();
            break;
        case TRANSFER_GUI:
            lv_create_transfer_gui();
            break;
        case TRANSFER_CONFIRM_GUI:
            lv_create_transfer_confirm_gui();
            break;
        case SETTINGS_GUI:
            lv_create_settings_gui();
            break;
        case ADMIN_CARD_GUI:
            lv_create_admin_card_gui();
            break;
        case PLAYER_CARD_GUI:
            lv_create_player_card_gui();
            break;
        default:
            // Handle invalid screen identifier
            break;
    }
}