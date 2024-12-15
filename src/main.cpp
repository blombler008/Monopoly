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

#include "config.hpp" 
#include "includes.hpp"
#include "macros.hpp"
#include "constants.hpp"
#include "custom_includes.hpp"

#include <vector>
#include <string>
#include <array>
#include <cstring>
 
#define tests 0


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

/**
 * @brief Creates a new instance of the SPIClass using the HSPI hardware SPI bus.
 * 
 * This line of code initializes a new SPIClass object and assigns it to the pointer `cls`.
 * The HSPI parameter specifies that the HSPI hardware SPI bus should be used.
 * 
 * @note Ensure that the HSPI bus is available and not being used by another peripheral.
 */
SPIClass* cls = new SPIClass(HSPI); 
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
SPIClass* vls = new SPIClass(VSPI); 

   
/**
 * @brief Initializes SPI settings with specified clock divider, bit order, and data mode.
 *
 * This instance of SPISettings configures the SPI communication with the following parameters:
 * - Clock Divider: SPI_CLOCK_DIV4 (sets the SPI clock to 1/4th of the system clock)
 * - Bit Order: MSBFIRST (Most Significant Bit First)
 * - Data Mode: SPI_MODE0 (Clock polarity 0, Clock phase 0)
 *
 * Clock Divider Options:
 * - SPI_CLOCK_DIV2: SPI clock is 1/2 of the system clock
 * - SPI_CLOCK_DIV4: SPI clock is 1/4 of the system clock
 * - SPI_CLOCK_DIV8: SPI clock is 1/8 of the system clock
 * - SPI_CLOCK_DIV16: SPI clock is 1/16 of the system clock
 * - SPI_CLOCK_DIV32: SPI clock is 1/32 of the system clock
 * - SPI_CLOCK_DIV64: SPI clock is 1/64 of the system clock
 * - SPI_CLOCK_DIV128: SPI clock is 1/128 of the system clock
 *
 * Bit Order Options:
 * - SPI_MSBFIRST: Most Significant Bit First
 * - SPI_LSBFIRST: Least Significant Bit First
 *
 * Data Mode Options:
 * - SPI_MODE0: Clock polarity 0, Clock phase 0
 * - SPI_MODE1: Clock polarity 0, Clock phase 1
 * - SPI_MODE2: Clock polarity 1, Clock phase 0
 * - SPI_MODE3: Clock polarity 1, Clock phase 1
 */

SPISettings spiSettings = SPISettings(SPI_CLOCK_DIV4, SPI_MSBFIRST, SPI_MODE0);

/**
 * @brief Initializes an MFRC522 SPI device.
 * 
 * This object represents an MFRC522 RFID reader connected via SPI interface.
 * 
 * @param RFID_CS1 Chip select pin for the RFID reader.
 * @param UNUSED_PIN Unused pin, typically set to a default or placeholder value.
 * @param vls Voltage level shifter or power control parameter.
 * @param spiSettings SPI settings configuration for the device.
 */
MFRC522_SPI spiDevice1 = MFRC522_SPI(RFID_CS1, UNUSED_PIN, vls, spiSettings); 

/**
 * @brief Initializes an MFRC522 SPI device with specified settings.
 * 
 * This object represents an MFRC522 RFID reader connected via SPI interface.
 * 
 * @param RFID_CS2 The chip select pin for the second RFID reader.
 * @param UNUSED_PIN A placeholder for an unused pin.
 * @param vls The voltage level shifter object or configuration.
 * @param spiSettings The SPI settings to be used for communication.
 */
MFRC522_SPI spiDevice2 = MFRC522_SPI(RFID_CS2, UNUSED_PIN, vls, spiSettings);  
  

/**
 * @brief Array of chip select (CS) pins for RFID modules.
 * 
 * This array holds the pin numbers used to select the RFID modules
 * connected to the microcontroller. The pins are defined by the 
 * RFID_CS_PINS macro.
 */
byte rfidCSPins[] = RFID_CS_PINS;
MFRC522 mfrc522[NR_OF_READERS] = {MFRC522(&spiDevice1),MFRC522(&spiDevice2)}; 
  
typedef void (*RFIDCallback)(const char* uid);
RFIDCallback rfid_callback = nullptr;

void set_rfid_callback(RFIDCallback callback) {
    rfid_callback = callback;
}

void clear_rfid_callback() {
    rfid_callback = nullptr;
}
void setupMain() {  
    Serial.begin(MONITOR_SPEED); // Initialize serial communication for debugging
    log_i("Starting"); // Log the start of the setup process
 
    vls->begin(VSPI_SCK, VSPI_MISO, VSPI_MOSI); // Initialize the VSPI bus
    cls->begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI); // Initialize the HSPI bus

    lv_setup_display(); // Initialize the display using LittlevGL
    
    // lv_create_remove_card_gui(card1); // Create the remove card GUI for testing
    // lv_create_add_card_gui(); // Create the add card GUI
    
    lv_create_start_gui(); // Create the initial graphical user interface
    lv_start_loop(); // Start the LittlevGL loop
    log_i("TFT setup"); // Log the completion of the display setup
    
    keypad_set_row_col_num(KEYPAD_ROWS, KEYPAD_COLS); // Set the number of rows and columns for the keypad
    keypad_set_pins((byte*)keypadColPins, (byte*)keypadRowPins); // Set the keypad pins
    keypad_set_layout((char*)keypadLayout); // Set the keypad layout
    keypad_setup(helper::keypadEvent); // Set up the keypad event handler
    
    pinMode(SD_CS, OUTPUT); // Set the SD card chip select pin as an output
    if(SD.begin(SD_CS, *cls, num_to_mhz(4))) { // Initialize the SD card on the HSPI bus [4 MHz]
        audio_setup(); // Set up the audio system
        audio_start_loop();  // Start the audio loop for service 
    } 
 
    for(int i=0; i<NR_OF_READERS; i++) {
        if(i>NR_OF_READERS) break; 
        mfrc522[i].PCD_Init();  // Init each MFRC522 reader
        delay(4); // Wait for the reader to initialize
        log_i("Reader %d(Pin %d): ", i, rfidCSPins[i]); // Log the reader number and pin
        mfrc522[i].PCD_DumpVersionToSerial(); // Dump the reader version to the serial monitor
        delay(4); // Wait for the reader to finish reading the version register 
    }
    
    log_i("Starting loop");
    
} 


void loopMain() { 
    for(int i=0; i<NR_OF_READERS; i++) { // Iterate over the readers
        if(i>NR_OF_READERS) break; // Break if the reader index exceeds the number of readers
        delay(20); // Delay to prevent rapid scanning

        // Check if a new card is present
        if (!mfrc522[i].PICC_IsNewCardPresent()) {
            continue;
        }

        // Attempt to read the card's serial number
        if (!mfrc522[i].PICC_ReadCardSerial()) {
            log_i("Reader %d(Pin %d): Bad read (was card removed too quickly?)", i, rfidCSPins[i]);  
            continue;
        }

        // Check if the UID size is valid
        if (mfrc522[i].uid.size == 0) {
            log_i("Reader %d(Pin %d): Bad card (size = 0)", i, rfidCSPins[i]);   
            continue;
        } 

        MFRC522::Uid uid = {0}; // Create a new UID object
        memcpy(&uid, &(mfrc522[i].uid), mfrc522[i].uid.size); // Copy the UID data
        char tag[20] = { 0 }; // Create a buffer for the tag data
        helper::format_uid_to_hex_string(tag, &uid, sizeof(tag)); // Dump the UID data into the buffer
        log_i("Reader %d(Pin %d): Good scan: %s", i, rfidCSPins[i], tag); // Log the UID data

        // Call the RFID callback if set
        if (rfid_callback) {
            rfid_callback(tag);
        }

        // Disengage the card
        mfrc522[i].PICC_HaltA();  
        mfrc522[i].PCD_StopCrypto1();
    }
}
#if tests == 0
void setup() {
    setupMain();
}
void loop() {
    loopMain();
}
#endif