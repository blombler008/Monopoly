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

// Touch + Display SPI PINS
#define HSPI_MISO    12 //11
#define HSPI_MOSI    13 //12
#define HSPI_SCK     14 // 9

// RFID SPI BUS
#define VSPI_MISO  48 //21  
#define VSPI_MOSI  47 //20  
#define VSPI_SCK   21 //19  

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
#define TFT_CS      10 // Chip select control pin
#define TFT_DC      11 // Data Command control pin 
#define TFT_RST     -1 //21  //47
#define TFT_CALLIBRATION_DATA { 178, 3717, 406, 3360, 7 } // Calibration data for the touch screen change this if needed
#define TOUCH_CS    1

// RFID Settings 
#define NR_OF_READERS 2
#define RFID_CS1    9
#define RFID_CS2    8 
#define RFID_CS_PINS { RFID_CS1, RFID_CS2 }

// SD Settings
#define SD_CS       4

// Audio Settings
#define I2S_MUTE    42
#define I2S_DOUT    40
#define I2S_BCLK    39
#define I2S_LRC     41

#define enable_keypad 0
// KEYPAD SETTINGS
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
