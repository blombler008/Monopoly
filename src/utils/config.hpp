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
 
// HSPI BUS === Touch + Display + RFID 
#define HSPI_MISO    12
#define HSPI_MOSI    13
#define HSPI_SCK     14

// VSPI BUS === SD CARD 
#define VSPI_MISO  15
#define VSPI_MOSI  16
#define VSPI_SCK   17

// I2C SETTINGS
#define I2C_SDA 6 // SDA pin for I2C communication
#define I2C_SCL 7 // SCL pin for I2C communication

// TFT SETTINGS 
#define TFT_SCREEN_WIDTH 320
#define TFT_SCREEN_HEIGHT 240
// TFT PINS
 
#define TFT_MISO HSPI_MISO
#define TFT_MOSI HSPI_MOSI
#define TFT_SCLK HSPI_SCK
#define TFT_CS      9 // Chip select control pin
#define TFT_DC      11 // Data Command control pin 
#define TFT_RST     -1 //21  //47 
#define TFT_CALLIBRATION_DATA { 250, 3351, 330, 3414, 1 }; // Calibration data for the touch screen change this if needed
#define TOUCH_CS    10

// RFID Settings  
#define RFID_CS     5  
#define RFID_SPI_SPEED num_to_mhz(4) // SPI speed for RFID communication (4 MHz)

// SD Settings
#define SD_CS       4
#define SD_DETECT_PIN 2
#define SD_STATUS_LED 8
#define SD_SPI_SPEED num_to_mhz(10) // SPI speed for SD card communication (10 MHz)

// Audio Settings
#define I2S_MUTE    42
#define I2S_DOUT    40
#define I2S_BCLK    39
#define I2S_LRC     41

// PHERIPHERALS SETTINGS
#define USE_KEYPAD 1
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
 
#define KEYPAD_LAYOUT "123A456B789C" "\x08" "0" "\x0D" "DNF"
// Replace \x08 and \x0D with the actual values of LV_KEY_BACKSPACE and LV_KEY_ENTER if they are defined as macros or constants elsewhere.
// Alternatively, if LV_KEY_BACKSPACE and LV_KEY_ENTER are macros, use stringification or define as a char array, e.g.:
 // static const char KEYPAD_LAYOUT[] = "123A456B789C" {LV_KEY_BACKSPACE, '0', LV_KEY_ENTER, 'D', 'N', 'F', '\0'};
 



#endif
