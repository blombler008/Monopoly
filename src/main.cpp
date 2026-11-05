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
 *   - CS Pin: RFID_CS
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
   
const char* keymap = KEYPAD_LAYOUT; // Define the keypad layout using the KEYPAD_LAYOUT macro

#endif // USE_KEYPAD

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

void create_gui() {
    lv_create_start_gui(); // Create the initial graphical user interface
    log_i("GUI created"); // Log the creation of the GUI
}

void setupMain() {  
    v_spi->begin(VSPI_SCK, VSPI_MISO, VSPI_MOSI); // Initialize the VSPI bus
    h_spi->begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI); // Initialize the HSPI bus 
    
    // Alle CS-Pins deaktivieren (HIGH = inaktiv)
    uint8_t CSPins[] = {TFT_CS, TOUCH_CS, RFID_CS, SD_CS}; // Array of chip select pins for various peripherals

    for (size_t i = 0; i < 4; i++) { 
        pinMode(CSPins[i], OUTPUT);
        digitalWrite(CSPins[i], HIGH);
    }
    
    xSemaphoreGive(spi_semaphore); // initial freigeben

#if USE_DISPLAY
    lv_setup_display(h_spi); // Initialize the display using LittlevGL   
    log_i("Display setup complete"); // Log the completion of the display setup
#endif

#if USE_RFID
    rfid_setup(h_spi); // Set up the RFID readers
    register_rfid_events();
    log_i("RFID setup complete"); // Log the completion of the RFID setup
#endif

#if USE_KEYPAD
    keypad_set_address(KEYPAD_I2C); // Set the I2C address for the keypad
    keypad_set_layout(keymap); // Set the layout for the keypad
    keypad_setup(); // Initialize the keypad with the specified layout and address
    log_i("Keypad initialized with address: 0x%02X", KEYPAD_I2C); // Log the I2C address of the keypad
#endif

#if USE_SD
    sd_setup(v_spi);
#endif  

#if USE_AUDIO
    audio_setup(); // Set up the audio system
    register_audio_events();
    audio_start_loop(AUDIO_RUNNING_CORE);  // Start the audio loop for service 
    log_i("Audio Driver initialized");
#endif 

    log_i("Starting loop"); 
} 
  
void setup() { 
    uint32_t oldCpuFreq = getCpuFrequencyMhz(); // Get the current CPU frequency in MHz
    setCpuFrequencyMhz(240);
    Serial.begin(MONITOR_SPEED); // Initialize serial communication for debugging
    log_i("Starting, CPU Frequency was: %lu MHz", oldCpuFreq); // Log the start of the setup process
 
    setupMain(); 

    serial_register_builtin_commands();
    serial_console_setup();

    create_gui();
}

#if tests == 0
 

void loop() { 
    // log_d("Loop begin");
    uint32_t now = esp_timer_get_time() / 1000; // Get the current time in milliseconds
    static uint32_t last_rfid = 0;
    static uint32_t last_sd = 0;
    static uint32_t last_loop = 0;

    serial_console_update();

#if USE_DISPLAY   // why is this in here? have to test // TODO:
    lv_timer_handler(); // Call the LittlevGL timer handler to update the display and handle events  
#endif

#if USE_RFID 
    if (now - last_rfid >= 200) {
        last_rfid = now; 
        rfid_loop(); 
       // rfid_loop(); // Call the RFID loop function to check for new cards 
    }
 
#endif 

#if USE_SD 
    if (now - last_sd >= 100) {
        last_sd = now; 
        sd_update();
    }
#endif
 
    // log_i("Loop delta at %lu ms", now-last_loop); // Log the time when the loop function is called
    last_loop = now; // Update the last loop time to the current time
}


#endif