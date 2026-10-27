#include "lv_setup_display.hpp"
#include "esp_heap_caps.h"
#include "drivers/lv_tft_espi_custom.h"

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

#define LOG_BUFFER_SIZE 8192
static char log_buffer[LOG_BUFFER_SIZE];
static volatile uint16_t log_write_pos = 0;

void lv_log(lv_log_level_t level, const char * buf) {
    // LV_UNUSED(level);
     uint16_t len = strlen(buf);

    if(len + log_write_pos >= LOG_BUFFER_SIZE)
        log_write_pos = 0; // wrap

    memcpy(&log_buffer[log_write_pos], buf, len);
    log_write_pos += len;
}

#define CHUNK_SIZE 1024
#include "driver/uart.h"

void log_task(void *param){
     uint16_t read_pos = 0;

    while(true)
    {
        if(read_pos != log_write_pos)
        {
            char temp[CHUNK_SIZE];
            int chunk = 0;

            while(read_pos != log_write_pos && chunk < CHUNK_SIZE)
            {
                temp[chunk++] = log_buffer[read_pos++];
                if(read_pos >= LOG_BUFFER_SIZE)
                    read_pos = 0;
            }

            uart_write_bytes(UART_NUM_0, temp, chunk);
        }

        vTaskDelay(1); // wichtig für Idle-Task
    }
}

void keypad_read_cb(lv_indev_t * indev, lv_indev_data_t* data) {
    static uint8_t last_key = 0; // Variable to store the last key pressed
    I2CKeyPad keypad = getKeypad(); // Get the keypad object

    if(!keypad.isPressed()) {last_key = 0; return;}; // Return if no key is pressed

    uint8_t key = keypad.getChar(); // Get the key pressed on the keypad 

    data->state = LV_INDEV_STATE_RELEASED; // Set the state to released
    data->key = key; // Set the key to the key pressed
 
    if(key == 8) {
        play_beeb_fast();
        goto labe;
    }
    if(key == 13) {
        play_beeb_high_fast();
        goto labe;
    }
    if(key && last_key != key) { 
        play_beeb_single(); 
    }
    labe:

    if (key) {
        data->state = LV_INDEV_STATE_PRESSED; // Check if the key is pressed
    }

    last_key = key; // Set the last key to the current key
}

void touch_read_cb(lv_indev_t * indev, lv_indev_data_t* data) {
    // log_i("Touch read callback called"); // Log when the touch read callback is called
    static uint16_t x, y;
 
    if( displayDriver->tft->dmaBusy() ) {
        // log_i("DMA is active, skipping touch read"); // Log when DMA is active and touch read is skipped
        // data->state = LV_INDEV_STATE_RELEASED;
        return;
    }
    
    xSemaphoreTake(spi_semaphore, pdMS_TO_TICKS(1));
    // log_i(displayDriver->tft->getTouch(&x, &y, 350) ? "Touch detected" : "No touch detected"); // Log whether a touch is detected or not
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
    xSemaphoreGive(spi_semaphore);
}
 
static uint32_t tick_wrapper(void) {
    return esp_timer_get_time() / 1000; // Return the current time in milliseconds
}

void lvgl_print_version() {
    char LVGL_Arduino[20]; // Create a buffer to hold the version string
    snprintf(LVGL_Arduino, sizeof(LVGL_Arduino), "LVGL V%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
    log_i("%s", LVGL_Arduino); // Log the version string to the serial output
}

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate(TFT_eSPI * tftref)  {

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


void lv_setup_display(SPIClass* _SPI) {
    init_lvgl_buffer(); // Initialize the LVGL buffer
    lvgl_print_version(); // Log the version of LVGL for compatibility
    uint16_t calData[5] = TFT_CALLIBRATION_DATA;
    lv_log_register_print_cb(lv_log); // Register the log callback function for LVGL
    xTaskCreate(log_task, "log_task", 16384, NULL, 1, NULL);
    lv_init(); // Initialize the LVGL library
    lv_log_register_print_cb(lv_log); // Register the log callback function for LVGL
    log_i("LVGL initialized"); // Log the initialization of LVGL
      
    lv_tick_set_cb(tick_wrapper); // Set the tick callback function for LVGL

    // Create a display using the TFT_eSPI library 
    pinMode(TFT_CS, OUTPUT); 
    displayInstance = lv_tft_espi_create(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT,  draw_buf_1, draw_buf_2, DMA_BUF_BYTES, _SPI); 
     
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

 
lv_display_t* display_get()
{
    return displayInstance;
}

 