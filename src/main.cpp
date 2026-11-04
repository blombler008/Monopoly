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
static bool s_sdAvailable = false;  // Flag to check if the SD card is ready to use

void make_bargraph(int numChars, float usedPercentage, char* label) {
    int num_hashes = static_cast<int>(usedPercentage / 2); 
    num_hashes = std::min(num_hashes, numChars);  
    std::string bar(num_hashes, '#'); 
    bar.append(numChars - num_hashes, ' ');  
    strcpy(label, bar.c_str()); 
}

void show_sd_info(bool more = true) {
    #define GB_MULTIPLIER (1024.0 * 1024.0 * 1024.0)
    log_i("SD Card Type: %s", SD.cardType() == CARD_SDHC ? "SDHC" : "SD");
    if(more) { 
        log_i("SD Card Size: %llu bytes", SD.cardSize());
        log_i("SD Card Sectors: %zu", SD.numSectors());
        log_i("SD Card Sector Size: %zu bytes", SD.sectorSize());
        log_i("SD Card Total Bytes: %llu", SD.totalBytes());
        log_i("SD Card Used Bytes: %llu", SD.usedBytes());
        log_i("SD Card Free Bytes: %llu", SD.totalBytes() - SD.usedBytes());
    }
    log_i("SD Card Used/Total GB: %.2f/%.2f", SD.usedBytes() / GB_MULTIPLIER, SD.totalBytes() / GB_MULTIPLIER);
    // Show used / total GB of the SD card as filling Bargraph from left to right ((SD.usedBytes() / SD.totalBytes()) * 100)
    // Like a progress bar, like this: [##########          ] 50%
    // width of the bar should be 40 characters
    float usedGB = SD.usedBytes() / GB_MULTIPLIER; // Convert used bytes to GB
    float totalGB = SD.totalBytes() / GB_MULTIPLIER; // Convert total bytes to GB
    float usedPercentage = (usedGB / totalGB) * 100.0; // Calculate the percentage of used space 
    char label[31]; // Create a label for the bar graph, 80 characters + null terminator
    make_bargraph(30, usedPercentage, (char*)label); // Create a bar graph string with 80 characters width
    label[30] = '\0'; // Ensure the label is null-terminated
    log_i("SD Card Used/Total GB: [%s] %.2f%%", label, usedPercentage); // Log the used percentage as a bar graph
    if(more) { 
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
}

bool sd_detect() { 
    return digitalRead(SD_DETECT_PIN) == HIGH;
}

void sd_update() {
    bool cardPresent = sd_detect();

    if (cardPresent == s_sdAvailable) return;

    if (cardPresent) {

        if (!SD.begin(SD_CS, *v_spi, SD_SPI_SPEED)) {
            log_e("Failed to initialize SD card");
            return;
        }

        show_sd_info(false);

        s_sdAvailable = true;

        fire_event(EventId::SDCardDetected);
        return;
    }

    SD.end();

    s_sdAvailable = false;

    fire_event(EventId::SDCardRemoved);
}
   
void setupMain() {  
    v_spi->begin(VSPI_SCK, VSPI_MISO, VSPI_MOSI); // Initialize the VSPI bus
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
    
    xSemaphoreGive(spi_semaphore); // initial freigeben

#if USE_DISPLAY
    lv_setup_display(h_spi); // Initialize the display using LittlevGL   
    log_i("Display setup complete"); // Log the completion of the display setup
    lv_create_start_gui(); // Create the initial graphical user interface
    log_i("GUI created"); // Log the creation of the GUI
    // lv_start_loop(); // Start the LittlevGL loop
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
    digitalWrite(SD_STATUS_LED, HIGH); // Turn on the SD status LED to indicate SD card operations 
    s_sdAvailable = sd_detect();
    if(s_sdAvailable){
        if(SD.begin(SD_CS, *v_spi, SD_SPI_SPEED)) { // Initialize the SD card on the VSPI bus [4 MHz] 
            log_i("SD Card initialized successfully!"); // Log success message for SD card initialization
            coreSettings.load();
                
            s_sdAvailable = true; // Set the flag indicating SD card is initialized
            fire_event(EventId::SDCardDetected); 
#if USE_AUDIO
            audio_setup(); // Set up the audio system
            audio_start_loop(AUDIO_RUNNING_CORE);  // Start the audio loop for service 
#endif 
            show_sd_info(); // Show information about the SD card
        } else {
            log_e("SD Card initialization failed!"); // Log an error if SD card initialization fails
            digitalWrite(SD_STATUS_LED, LOW); // Turn off the SD status LED to indicate failure
        } 
    }
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
}

#if tests == 0
unsigned long last_display_time = 0;
unsigned long last_rfid_time = 0;
 
void toggle_led() {

    static bool lastState = false;

    if (!s_sdAvailable) {
        if (lastState) {
            digitalWrite(SD_STATUS_LED, LOW);
            lastState = false;
        }
        return;
    }

    uint32_t phase = (esp_timer_get_time() / 1000) % 500;
    bool newState = (phase < 50);

    if (newState != lastState) {
        // if(newState) 
            // log_i("LED state changed to %s", newState ? "HIGH" : "LOW");
        digitalWrite(SD_STATUS_LED, newState ? HIGH : LOW);
        lastState = newState;
    }
}
 

void loop() { 
    // log_d("Loop begin");
    uint32_t now = esp_timer_get_time() / 1000; // Get the current time in milliseconds
    static uint32_t last_sd = 0;
    static uint32_t last_rfid = 0;
    static uint32_t last_led = 0;
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

    last_led = now;
    toggle_led();

    // log_i("Loop delta at %lu ms", now-last_loop); // Log the time when the loop function is called
    last_loop = now; // Update the last loop time to the current time
}


#endif