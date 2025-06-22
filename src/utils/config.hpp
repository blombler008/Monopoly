#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef ota
    #define ota 0
#endif
#ifndef tests
    #define tests 0
#endif

// MONITOR SEETINGS
#define MONITOR_SPEED MONITOR_BAUD_7

// SPI SETTINGS
#define VSPI_SPEED num_to_mhz(10)

// Touch + Display SPI PINS + RFID SPI BUS 
#define HSPI_MISO    12
#define HSPI_MOSI    13
#define HSPI_SCK     14

// SD CARD SPI BUS
#define FSPI_MISO  15
#define FSPI_MOSI  16
#define FSPI_SCK   17

// TFT SETTINGS
#define TFT_ROTATION TFT_LANDSCAPE_1
#define TFT_SCREEN_WIDTH 240
#define TFT_SCREEN_HEIGHT 320
// TFT PINS

#define TFT_MISO HSPI_MISO
#define TFT_MOSI HSPI_MOSI
#define TFT_SCLK HSPI_SCK
#define TFT_CS      9 // Chip select control pin
#define TFT_DC      11 // Data Command control pin 
#define TFT_RST     -1 //21  //47
#define TFT_CALLIBRATION_DATA { 178, 3717, 406, 3360, 7 } // Calibration data for the touch screen change this if needed
#define TOUCH_CS    10

// RFID Settings  
#define RFID_CS     5 

// SD Settings
#define SD_CS       4
#define SD_DETECT_PIN 2
#define SD_STATUS_LED 8

// Audio Settings
#define I2S_MUTE    42
#define I2S_DOUT    40
#define I2S_BCLK    39
#define I2S_LRC     41

// PHERIPHERALS SETTINGS
#define USE_KEYPAD 0
#define USE_DISPLAY 1
#define USE_RFID 1
#define USE_SD 1
#if USE_SD
    #define USE_AUDIO 1
#else
    #define USE_AUDIO 0
#endif

// KEYPAD SETTINGS

#define KEYPAD_I2C 0x20 // default i2c address of the pfc8574 i2c expander (0x20, assuming all pins are connected to GND)
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define KEYPAD_LAYOUT { \
    {'1','4','7',LV_KEY_BACKSPACE}, \
    {'2','5','8','0'}, \
    {'3','6','9',LV_KEY_ENTER}, \
    {'A','B','C','D'}  \
}
#define KEYPAD_ROW_PINS //{4, 5, 6, 7}
#define KEYPAD_COL_PINS //{15, 16, 17, 18}

#endif
