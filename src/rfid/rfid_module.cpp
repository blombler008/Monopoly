#include "rfid_module.hpp"

using Uid = MFRC522::Uid;
/**
 * @brief Initializes an MFRC522 SPI device.
 * 
 * This object represents an MFRC522 RFID reader connected via SPI interface.
 * 
 * @param RFID_CS1 Chip select pin for the RFID reader.
 * @param UNUSED_PIN Unused pin, typically set to a default or placeholder value.
 * @param vls Voltage level shifter or power control parameter. 
 */
MFRC522_SPI* spiDevice1; 

/**
 * @brief Array of chip select (CS) pins for RFID modules.
 * 
 * This array holds the pin numbers used to select the RFID modules
 * connected to the microcontroller. The pins are defined by the 
 * RFID_CS_PINS macro.
 */

MFRC522* mfrc522; 

RFIDCallback rfid_callback = nullptr;

void set_rfid_callback(RFIDCallback callback) {
    rfid_callback = callback;
}

void clear_rfid_callback() {
    rfid_callback = nullptr;
}
 
void rfid_setup(SPIClass* v_spi) {
    spiDevice1 = new MFRC522_SPI(RFID_CS, UNUSED_PIN, v_spi); // Initialize the MFRC522 SPI device with the specified chip select pin and unused pin
    mfrc522 = new MFRC522(spiDevice1);
    // Initialize the RFID reader
    mfrc522->PCD_Init();  // Initialize the MFRC522 reader
    mfrc522->PCD_DumpVersionToSerial(); // Dump the version of the MFRC522 reader to the serial console
    delay(10); // Wait for the reader to initialize   

    uint8_t version = mfrc522->PCD_ReadRegister(mfrc522->VersionReg); // Store the version in a byte variable

    while (version == 0x00) { // Check if the reader is present
        log_i("RFID Reader (Pin %d): Not found: 0x%02x", RFID_CS, version); // Log if the reader is not found 
        mfrc522->PCD_Init();  // Reinitialize the MFRC522 reader
        mfrc522->PCD_DumpVersionToSerial(); // Dump the version of the MFRC522 reader to the serial console
        delay(10); // Wait for the reader to initialize   
        version = mfrc522->PCD_ReadRegister(mfrc522->VersionReg); // Store the version in a byte variable
    } 
    log_i("RFID Reader (Pin %d): Found", RFID_CS); // Log that the reader was found
}


// make an array like objects to compare the tag with 
// predefined tags for intro, haut, and show
// objects are predefiled 4bytes-string (like "0x22-0x67-0x39-0x0") mapped to a string... content of the String is the audio file to play
// These tags are used to trigger specific audio playback 
// The tags are defined as strings for easy comparison
    
typedef struct {
    const char* tag; // The tag string
    const char* audio_file; // The audio file to play when the tag is detected
} RFIDTag;

bool compare_tag(const char* tag, const RFIDTag* rfid_tags, size_t num_tags) {
    for (size_t i = 0; i < num_tags; i++) {
        if (strcmp(tag, rfid_tags[i].tag) == 0) {
            
            log_i("RFID Reader (Pin %d): %s tag detected", RFID_CS,  rfid_tags[i].audio_file); // Log the detected tag
            audio_playback_start(rfid_tags[i].audio_file); // Start playing the audio file associated with the tag
            return true; // Return true if a match is found
        }
    }
    return false; // Return false if no match is found
}

const RFIDTag rfid_tags[] = {
    {"0x22-0x67-0x39-0x0", "Intro.mp3"},
    {"0x59-0xab-0xc9-0x0", "Haut.mp3"},
    {"0xde-0xef-0xe2-0x0", "Show.mp3"},

    // Add more tags and their corresponding audio files here 
    // {"0xXX-0xXX-0xXX-0x0", "AudioFile.mp3"}, // Example of another tag
    {"0x1e-0x62-0x2c-0x0", "Brot.mp3"},
    {"0x6e-0xa8-0xe5-0x0", "HypaHypa.mp3"},
};

void rfid_loop() {
    // log_i("RFID Reader (Pin %d): Waiting for card...", RFID_CS); // Log that the reader is waiting for a card
    // Check if a new card is present 
    if (!mfrc522->PICC_IsNewCardPresent()) {
        return; // If no new card is present, exit the loop
    }; // If no new card is present, exit the loop
    
    // Attempt to read the card's serial number
    if (!mfrc522->PICC_ReadCardSerial()) {
        log_i("RFID Reader (Pin %d): Bad read (was card removed too quickly?)", RFID_CS);   
        return;
    }

    // Check if the UID size is valid
    if (mfrc522->uid.size == 0) {
        log_i("RFID Reader (Pin %d): Bad card (size = 0)", RFID_CS);   
        
        xSemaphoreGive(spi_semaphore);
        return;
    } 

    Uid uid = {0}; // Create a new UID object
    memcpy(&uid, &(mfrc522->uid), mfrc522->uid.size); // Copy the UID data
    char tag[20] = {0}; // Create a buffer for the tag data

    format_uid_to_hex_string(tag, &uid, sizeof(tag)); // Dump the UID data into the buffer
    
    log_i("RFID Reader (Pin %d): Good scan: %s", RFID_CS, tag); // Log the UID data


    // Check if the tag matches any predefined tags
    if (compare_tag(tag, rfid_tags, sizeof(rfid_tags) / sizeof(RFIDTag))) { 
        // return;
    } else {
        log_i("RFID Reader (Pin %d): No matching tag found: \"%s\"", RFID_CS, tag); // Log if no matching tag is found
    }

    // Call the RFID callback if set
    if (rfid_callback) rfid_callback(tag);
    

    // Disengage the card
    mfrc522->PICC_HaltA();  
    mfrc522->PCD_StopCrypto1();  
}