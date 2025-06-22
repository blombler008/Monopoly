/**
 * @file main.cpp
 * @brief Main source file for the Monopoly project using ESP32-S3-DevKitC-1.
 * 
 * This file contains the setup and loop functions for initializing and running the Monopoly game.
 * It includes configurations for keypad input, SPI communication, RFID readers, and audio playback.
 * 
 * Pinout scheme for ESP32-S3-DevKitC-1:
 * 
 * Keypad:
 * - Row Pins: 4, 5, 6, 7
 * - Column Pins: 15, 16, 17, 18
 * 
 * SPI:
 * - HSPI (High-Speed SPI):
 *   - SCK: HSPI_SCK
 *   - MISO: HSPI_MISO
 *   - MOSI: HSPI_MOSI
 * - VSPI (Default SPI):
 *   - SCK: VSPI_SCK
 *   - MISO: VSPI_MISO
 *   - MOSI: VSPI_MOSI
 * 
 * RFID Readers:
 * - Reader 1:
 *   - CS Pin: RFID_CS1
 * - Reader 2:
 *   - CS Pin: RFID_CS2
 * 
 * SD Card:
 * - CS Pin: SD_CS
 * 
 * Keypad Layout:
 * - 4x4 matrix with the following layout:
 *   1  2  3  A
 *   4  5  6  B
 *   7  8  9  C
 *   *  0  #  D
 * 
 * Functions:
 * - setup(): Initializes the serial communication, SPI interfaces, display, keypad, SD card, and RFID readers.
 * - loop(): Continuously checks for new RFID card scans and handles keypad events.
 */
#include "main.hpp"

bool stopLoop = false;  
 
#if USE_KEYPAD
/**
 * @brief Array of pin numbers corresponding to the rows of the keypad.
 * 
 * This array is used to define which pins on the microcontroller are connected
 * to the rows of the keypad. The size of the array is determined by the 
 * KEYPAD_ROWS constant, and the pin numbers are defined by the KEYPAD_ROW_PINS 
 * macro.
 */
const byte keypadRowPins[KEYPAD_ROWS] = KEYPAD_ROW_PINS;
/**
 * @brief Array of pin numbers for the columns of the keypad.
 * 
 * This array holds the pin numbers that are connected to the columns of the keypad.
 * The size of the array is determined by the KEYPAD_COLS constant.
 * The pin numbers are defined by the KEYPAD_COL_PINS macro.
 */
const byte keypadColPins[KEYPAD_COLS] = KEYPAD_COL_PINS;  

/**
 * @brief Layout of the keypad.
 * 
 * This 2D array represents the layout of a 4x4 keypad. Each element corresponds
 * to a key on the keypad.
 * 
 * @var KEYPAD_LAYOUT The 2D array representing the keypad layout.
 * @var KEYPAD_ROWS The number of rows in the keypad.
 * @var KEYPAD_COLS The number of columns in the keypad.
 */
const char keypadLayout[KEYPAD_ROWS][KEYPAD_COLS] = KEYPAD_LAYOUT;
#endif
/**
 * @brief Creates a new instance of the SPIClass using the HSPI hardware SPI bus.
 * 
 * This line of code initializes a new SPIClass object and assigns it to the pointer `h_spi`.
 * The HSPI parameter specifies that the HSPI hardware SPI bus should be used.
 * 
 * @note Ensure that the HSPI bus is available and not being used by another peripheral.
 */
SPIClass* h_spi = new SPIClass(HSPI); 
/**
 * @brief Creates a new instance of the SPIClass using the VSPI hardware SPI bus.
 * 
 * This line of code initializes a pointer to an SPIClass object, which is 
 * configured to use the VSPI (Virtual SPI) bus. The VSPI bus is one of the 
 * hardware SPI buses available on the ESP32 microcontroller.
 * 
 * @note Ensure that the VSPI bus is not being used by other peripherals 
 *       before initializing it to avoid conflicts.
 */
SPIClass* v_spi = new SPIClass(VSPI); 

void make_bargraph(int numChars, float usedPercentage, char* label) {
    int num_hashes = static_cast<int>(usedPercentage / 2); 
    num_hashes = std::min(num_hashes, numChars);  
    std::string bar(num_hashes, '#'); 
    bar.append(numChars - num_hashes, ' ');  
    strcpy(label, bar.c_str()); 
}

void show_sd_info() {
    log_i("SD Card Type: %s", SD.cardType() == CARD_SDHC ? "SDHC" : "SD");
    log_i("SD Card Size: %llu bytes", SD.cardSize());
    log_i("SD Card Sectors: %zu", SD.numSectors());
    log_i("SD Card Sector Size: %zu bytes", SD.sectorSize());
    log_i("SD Card Total Bytes: %llu", SD.totalBytes());
    log_i("SD Card Used Bytes: %llu", SD.usedBytes());
    log_i("SD Card Free Bytes: %llu", SD.totalBytes() - SD.usedBytes());
    log_i("SD Card Used/Total GB: %.2f/%.2f", SD.usedBytes() / (1024.0 * 1024.0 * 1024.0), SD.totalBytes() / (1024.0 * 1024.0 * 1024.0));
    // Show used / total GB of the SD card as filling Bargraph from left to right ((SD.usedBytes() / SD.totalBytes()) * 100)
    // Like a progress bar, like this: [##########          ] 50%
    // width of the bar should be 40 characters
    float usedGB = SD.usedBytes() / (1024.0 * 1024.0 * 1024.0); // Convert used bytes to GB
    float totalGB = SD.totalBytes() / (1024.0 * 1024.0 * 1024.0); // Convert total bytes to GB
    float usedPercentage = (usedGB / totalGB) * 100.0; // Calculate the percentage of used space 
    char label[81]; // Create a label for the bar graph, 80 characters + null terminator
    make_bargraph(80, usedPercentage, (char*)label); // Create a bar graph string with 80 characters width
    label[80] = '\0'; // Ensure the label is null-terminated
    log_i("SD Card Used/Total GB: [%s] %.2f%%", label, usedPercentage); // Log the used percentage as a bar graph

    log_i("SD contents:"); // Log the contents of the SDSS file system
    File root = SD.open("/"); // Open the root directory of SDFS
    if (root) {
        File file = root.openNextFile(); // Open the next file in the directory
        while (file) { // While there are files in the directory
            log_i("File: %s, Size: %d bytes", file.name(), file.size()); // Log the file name and size
            file = root.openNextFile(); // Move to the next file
        }
        root.close(); // Close the root directory
    } else {
        log_i("Failed to open SD root directory"); // Log failure to open root directory
    }
}
   
bool is_sd_card_initialized = false; // Flag to check if the SD card is initialized
void setupMain() {  
    v_spi->begin(FSPI_SCK, FSPI_MISO, FSPI_MOSI); // Initialize the VSPI bus
    h_spi->begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI); // Initialize the HSPI bus 
 
    pinMode(SD_STATUS_LED, OUTPUT); // Set the SD status LED pin as output 
    digitalWrite(SD_STATUS_LED, LOW); // Turn off the SD status LED initially
    pinMode(SD_DETECT_PIN, INPUT_PULLUP); // Set the SD card detect pin as input with pull-up resistor 

    // Alle CS-Pins deaktivieren (HIGH = inaktiv)
    uint8_t CSPins[] = {TFT_CS, TOUCH_CS, RFID_CS, SD_CS}; // Array of chip select pins for various peripherals

    for (size_t i = 0; i < 4; i++) { 
        pinMode(CSPins[i], OUTPUT);
        digitalWrite(CSPins[i], HIGH);
    }
 
#if USE_DISPLAY
 
    lv_setup_display(); // Initialize the display using LittlevGL  
    lv_create_start_gui(); // Create the initial graphical user interface
    lv_start_loop(); // Start the LittlevGL loop
#endif
#if USE_RFID
    rfid_setup(h_spi); // Set up the RFID readers
#endif

#if USE_KEYPAD
    keypad_set_row_col_num(KEYPAD_ROWS, KEYPAD_COLS); // Set the number of rows and columns for the keypad
    keypad_set_pins((byte*)keypadColPins, (byte*)keypadRowPins); // Set the keypad pins
    keypad_set_layout((char*)keypadLayout); // Set the keypad layout
    keypad_setup(); // Set up the keypad event handler
#endif
#if USE_SD
    digitalWrite(SD_STATUS_LED, HIGH); // Turn on the SD status LED to indicate SD card operations 
    if(SD.begin(SD_CS, *v_spi, num_to_mhz(4))) { // Initialize the SD card on the HSPI bus [4 MHz]
        is_sd_card_initialized = true; // Set the flag indicating SD card is initialized
        log_i("SD Card initialized successfully!"); // Log success message for SD card initialization
#if USE_AUDIO
        audio_setup(); // Set up the audio system
        audio_start_loop();  // Start the audio loop for service 
#endif 
        show_sd_info(); // Show information about the SD card
    } else {
        log_e("SD Card initialization failed!"); // Log an error if SD card initialization fails
        digitalWrite(SD_STATUS_LED, LOW); // Turn off the SD status LED to indicate failure
    } 
#endif  
    log_i("Starting loop"); 
} 
 
#if tests == 0
void setup() { 
 
    Serial.begin(MONITOR_SPEED); // Initialize serial communication for debugging
    log_i("Starting"); // Log the start of the setup process
 
    setupMain(); 
}

unsigned long last_display_time = 0;
unsigned long last_rfid_time = 0;
bool ledChanged = false; // Flag to track if the LED state has changed
bool ledState = false; // Current state of the LED (off by default)
void loop() {
    
    vTaskDelay(20); 
    unsigned long now = millis();

    // make the sd led blink in an interval of 500ms like a heartbeat (off for 450ms, on for 50ms)
    //only write to the SD status LED when the SD card is initialized and the led does not need to change state
    // eg. when the SD card is not initialized, the LED should be off
    // eg. when the state is off, no need to write to the LED off
    // eg. when the state is on, no need to write to the LED on
    // this is to save power and avoid unnecessary writes to the LED

    if (is_sd_card_initialized) {
        if (ledChanged) {
            digitalWrite(SD_STATUS_LED, ledState ? HIGH : LOW); // Set the SD status LED to the current state
            ledChanged = false; // Reset the flag after changing the LED state
        }
        if (now % 500 < 50) {
            ledState = true; // Set the LED state to on
            ledChanged = true; // Set the flag to indicate that the LED state has changed
        } else {
            ledState = false; // Set the LED state to off
            ledChanged = true; // Set the flag to indicate that the LED state has changed 
        }
    } 
 
#if USE_RFID
    // 100ms RFID
    if (now - last_rfid_time >= 100) {
        last_rfid_time = now;
 
        rfid_loop(); // Call the RFID loop function to check for new cards 
    }

#endif
 
#if USE_DISPLAY 
    lv_task_handler();  // let the GUI do its work
    lv_timer_handler(); 
#endif
    if(stopLoop) {
        return;
    }  
}
#endif