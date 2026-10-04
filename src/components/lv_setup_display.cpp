#include "lv_setup_display.hpp"
#include "esp_heap_caps.h"

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
    lv_display_t * disp;
    bool dma_busy;
} lv_tft_espi_t;

lv_obj_t* textInput;
lv_obj_t* keyboard;
 
lv_tft_espi_t* displayDriver;
lv_display_t * displayInstance;
lv_indev_t * touchInputDevice;
lv_indev_t * keypadInputDevice;
lv_group_t* keypadGroup;
lv_timer_t* timer;  
uint16_t colors[] = {ILI9341_PURPLE, ILI9341_RED, ILI9341_BLUE, ILI9341_GREEN, ILI9341_ORANGE, ILI9341_YELLOW, ILI9341_CYAN, ILI9341_MAGENTA, ILI9341_WHITE};

#define DTFT_WIDTH        320
#define DTFT_HEIGHT       240   
#define DMA_BUF_BYTES    (DTFT_WIDTH * DTFT_HEIGHT * 2) / 8 // buffer size in bytes, for 240 lines of the display (320*240*2 bytes for 16-bit color depth)
static lv_color16_t* draw_buf_1 = nullptr;
static lv_color16_t* draw_buf_2 = nullptr; 

static size_t pixel_count = DMA_BUF_BYTES / sizeof(lv_color16_t); 


static lv_tft_espi_t* g_disp = nullptr;
static volatile bool dma_active = false;

static void dma_timer_cb(void* pvParameters) {
    
    for (;;)  {       
        if (dma_active && !g_disp->tft->dmaBusy()) {
            dma_active = false;

            g_disp->tft->endWrite();

            lv_disp_flush_ready(g_disp->disp);
        }

        vTaskDelay(pdMS_TO_TICKS(1));   // 1ms Polling reicht
    } 
}

// getbuffer
lv_color16_t* getNewBuffer(){
    return (lv_color16_t*)heap_caps_malloc(
        DMA_BUF_BYTES,
        // MALLOC_CAP_DMA
        // MALLOC_CAP_8BIT
        // MALLOC_CAP_DEFAULT 
        // MALLOC_CAP_INTERNAL
        // MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA
        // MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT
        // MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA | MALLOC_CAP_8BIT
        MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA | MALLOC_CAP_8BIT
    );
}

void init_lvgl_buffer() { 

    log_i("Free INTERNAL: %u", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    log_i("Free DMA: %u", heap_caps_get_free_size(MALLOC_CAP_DMA));
    log_i("Allocation per buffer: %u bytes, total %u bytes", DMA_BUF_BYTES, DMA_BUF_BYTES * 2);
    draw_buf_1 = getNewBuffer();
    draw_buf_2 = getNewBuffer();

    if (!draw_buf_1 || !draw_buf_2) {
        log_e("DMA allocation failed!");
        while (true);
    }
    log_i("Buf1 addr: %p", draw_buf_1);
    log_i("Buf2 addr: %p", draw_buf_2);

    log_i("LVGL buffer allocated in DMA memory");
}



void lv_log(lv_log_level_t level, const char * buf) {
    LV_UNUSED(level);
    log_i("LVGL: %s", buf); 
}
 
void keypad_read_cb(lv_indev_t * indev, lv_indev_data_t* data) {
    static uint8_t last_key = 0; // Variable to store the last key pressed
    I2CKeyPad keypad = getKeypad(); // Get the keypad object

    if(!keypad.isPressed()) return; // Return if no key is pressed

    uint8_t key = keypad.getChar(); // Get the key pressed on the keypad 

    // if the same key is pressed as the last key, we want to ignore the press if it is withing a 2 sec window. 
    //otherwise when the key is released, it will be considered a new key press
    // if (key == last_key && millis() - keypad.getLastTimeRead() < 2000) {
    //     data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
    //     return; // Ignore the key press
    // }

    data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
    data->key = key; // Set the key to the key pressed
 
    if (key) {
        data->state = LV_INDEV_STATE_PRESSED; // Check if the key is pressed
    }

    last_key = key; // Set the last key to the current key
}

void touch_read_cb(lv_indev_t * indev, lv_indev_data_t* data)
{
    static uint16_t x, y;
 
    if( display_dma_is_active() ) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    if(displayDriver->tft->getTouch(&x, &y, 350)) { 
        // Invert the touch coordinates to match the display orientation and swap x and y
        data->point.x = x;
        data->point.y = y; // Invert y coordinate to match display orientation

        // draw a crosshair at the touch point for debugging
            // displayDriver->tft->drawLine(x - 10, y, x + 10, y, TFT_RED);
            // displayDriver->tft->drawLine(x, y - 10, x, y + 10, TFT_RED);
        // also draw lvgl's idea of the touch point for debugging
        //    static lv_obj_t * dot;

        //     dot = lv_obj_create(lv_screen_active());
        //     lv_obj_set_size(dot, 8, 8);
        //     lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        //     lv_obj_set_style_bg_color(dot, lv_color_hex(0x00FF00), 0);
        //     lv_obj_set_style_border_width(dot, 0, 0);

        //     lv_obj_set_pos(dot, data->point.x - 4, data->point.y - 4);
        // log_i("Converted Touch: x=%d\t, y=%d\t", data->point.x, data->point.y); // Log the converted touch coordinates to the serial output
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// void touch_read_cb(lv_indev_t * indev, lv_indev_data_t* data) { 
//     uint16_t touchpad_x, touchpad_y; // Variables to store the touchpad coordinates
 
//     bool touchpad_pressed = displayDriver->tft->getTouch(&touchpad_x, &touchpad_y, 300); // Get the touchpad coordinates 
//     // log_i("Touch: x=%d, y=%d, pressed=%d", touchpad_x, touchpad_y, touchpad_pressed); // Log the touch coordinates and state to the serial output
//     // uint16_t raw_touchpad_x, raw_touchpad_y; // Variables to store the touchpad coordinates
//     // bool raw_touchpad_pressed = displayDriver->tft->getTouchRaw(&raw_touchpad_x, &raw_touchpad_y); // Get the raw touchpad coordinates
//     // log_i("Raw Touch: x=%d, y=%d, pressed=%d", raw_touchpad_x, raw_touchpad_y, raw_touchpad_pressed); // Log the raw touch coordinates and state to the serial output
    
//     if (touchpad_pressed) { // Check if the touchpad is pressed
//         data->point.y = touchpad_x; // swap x and y
//         data->point.x = 240 - touchpad_y;  // swap x and y and invert x
//         data->state = LV_INDEV_STATE_PRESSED; // Set the state to pressed
//     } else {
//         data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
//     }

//     // log_i("Touch: x=%d, y=%d, state=%d", data->point.x, data->point.y, data->state); // Log the touch coordinates and state to the serial output
    
// }

static uint32_t tick_wrapper(void) {
    return esp_timer_get_time() / 1000; // Return the current time in milliseconds
}

void lvgl_print_version() {
    char LVGL_Arduino[20]; // Create a buffer to hold the version string
    snprintf(LVGL_Arduino, sizeof(LVGL_Arduino), "LVGL V%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
    log_i("%s", LVGL_Arduino); // Log the version string to the serial output
}

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate(TFT_eSPI * tftref)
{

  TFT_eSPI tft = *tftref; 
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}

void lv_setup_display(void) {
    init_lvgl_buffer(); // Initialize the LVGL buffer
    lvgl_print_version(); // Log the version of LVGL for compatibility
    uint16_t calData[5] = TFT_CALLIBRATION_DATA; 
    lv_init(); // Initialize the LVGL library
    log_i("LVGL initialized"); // Log the initialization of LVGL
      
    lv_tick_set_cb(tick_wrapper); // Set the tick callback function for LVGL

    // Create a display using the TFT_eSPI library 
    displayInstance = lv_tft_espi_create(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT,  draw_buf_1, draw_buf_2, DMA_BUF_BYTES);  
    
    // lv_timer_create(dma_timer_cb, 1, NULL); // Create a timer to check the DMA status every 5 ms
    displayDriver = (lv_tft_espi_t*)lv_display_get_driver_data(displayInstance); // Get the display driver data
    
    if (displayDriver == NULL) {
        log_e("Failed to get display driver data");
        throw std::runtime_error("Failed to get display driver data");
    }

    g_disp = (lv_tft_espi_t*)displayDriver; // Set the global display instance for the DMA timer callback
    // lv_display_set_rotation(displayInstance, LV_DISPLAY_ROTATION_90); // Set the display rotation 
    // touch_calibrate(displayDriver->tft); // Calibrate the touch screen and get the calibration data
    // ESP.restart(); 
  
    log_i("TFT setup"); // Log the completion of the display setup  
    lv_display_set_rotation(displayInstance, LV_DISPLAY_ROTATION_0);
    displayDriver->tft->setTouch(calData); // Calibrate the touch screen using the predefined calibration data  
    log_i("Touch screen calibrated"); // Log the calibration of the touch screen

    touchInputDevice = lv_indev_create(); // Create a new input device
    lv_indev_set_type(touchInputDevice, LV_INDEV_TYPE_POINTER); // Set the input device type to pointer (touchpad)
    lv_indev_set_read_cb(touchInputDevice, touch_read_cb); // Set the read callback function for the input device
    log_i("Touch input device created"); // Log the creation of the touch input device

    keypadInputDevice = lv_indev_create(); // Create a new input device
    lv_indev_set_type(keypadInputDevice, LV_INDEV_TYPE_KEYPAD); // Set the input device type to keypad 
    lv_indev_set_read_cb(keypadInputDevice, keypad_read_cb); // Set the read callback function for the input device
    log_i("Keypad input device created"); // Log the creation of the keypad input device

    keypadGroup = lv_group_create(); // Create a new group for the keypad
    lv_indev_set_group(keypadInputDevice, keypadGroup); // Create a new group for the keypad input device
    lv_group_set_default(keypadGroup); // Set the default group for the keypad input device
    log_i("Keypad group created"); // Log the creation of the keypad group
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

void time_gui_creation_async(void (*gui_function)(void*), void* param) {
    unsigned long start_time = portGET_RUN_TIME_COUNTER_VALUE();
    lv_async_call(gui_function, param);
    unsigned long end_time = portGET_RUN_TIME_COUNTER_VALUE();
    log_i("GUI creation time: %lu ns", (end_time - start_time)*4);
}

void lv_screen_switch(GuiScreens screen, void* user_data) {
    delete_timer(); // Delete the timer

    clear_rfid_callback(); // Clear the RFID callback

    // Clear the current screen
    lv_obj_clean(lv_screen_active());
    
    delay(10); // Delay to allow the screen to clear

    // Switch to the specified screen
    lv_create_start_gui();
}

void remove_keyboard_and_clear_focus() {
    if(keyboard != nullptr) { 
        lv_obj_delete(keyboard);
        keyboard = nullptr;
    }  
    if(textInput != nullptr) {
        lv_obj_remove_state(textInput, LV_STATE_FOCUSED);
        textInput = nullptr;
    }
 
}

void show_keyboard_on_click(lv_event_t * event, lv_obj_t* textInputX) {
    lv_event_code_t eventCode = lv_event_get_code(event);
    textInput = textInputX;

    switch (eventCode) {
        case LV_EVENT_FOCUSED: 
            keyboard = lv_keyboard_create(lv_screen_active());
            lv_keyboard_set_textarea(keyboard, textInput);  
            break;

        case LV_EVENT_READY:
        case LV_EVENT_DEFOCUSED:
            remove_keyboard_and_clear_focus();
            // Call the appropriate function to handle the event
            break;
 
        default:
            break;
    }
}



lv_obj_t* lv_obj_assert_null(lv_obj_t* obj) {
    if (obj == NULL) {
        log_e("Object is NULL");
        throw std::runtime_error("Object is NULL");
        return NULL;
    }
    if(!lv_obj_is_valid(obj)) {
        log_e("Object is not valid");
        throw std::runtime_error("Object is not valid");
        return NULL;
    } 
    return obj;
}


bool display_dma_is_active()
{
    return g_disp->tft->dmaBusy();
}

lv_display_t* display_get()
{
    return displayInstance;
}

void display_dma_poll()
{
    if (g_disp == nullptr)
        return;
    
    if (!display_dma_is_active()) { 
        g_disp->tft->endWrite(); 
    } 
}