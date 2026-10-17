#include "rfid_module.hpp"


/**
 * @brief Flag to enable or disable RFID scanning.
 * This volatile boolean variable is used to control whether the RFID scanning loop should be active.
 * It is set to true when scanning is enabled and false when scanning is disabled, allowing for dynamic control of the RFID scanning process.
 * 
 */
volatile bool rfid_scan_enable = false;

/**
 * @brief Flag to enable or disable audio tag functionality.
 * This volatile boolean variable is used to control whether the audio tag functionality should be active.
 */
volatile bool rfid_audio_tags_enable = false;


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
 * @brief Structure representing an RFID tag and its associated audio file.
 */
typedef struct {
    const char* tag; // The tag string
    const char* audio_file; // The audio file to play when the tag is detected
} RFIDTag;

/**
 * @brief Array of RFID tags and their associated audio files.
 */

static RFIDTag* rfid_tags = nullptr;

/**
 * @brief Number of RFID tags in the array.
 */
static size_t rfid_tags_count = 0;

/**
 * @brief Type alias for the UID structure used by the MFRC522 library.
 */
using Uid = MFRC522::Uid;

/**
 * @brief Pointer to the MFRC522 RFID reader object.
 * 
 * This pointer is used to interact with the MFRC522 RFID reader, allowing for operations such as scanning for tags, reading tag data, and controlling the reader's behavior.
 */
MFRC522* mfrc522; 

/**
 * @brief Callback function for handling RFID tag detections.
 * 
 * This function is called when an RFID tag is detected by the reader.
 */
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
    

bool compare_tag(const char* tag) {
    for (size_t i = 0; i < rfid_tags_count; i++) {
        if (strcmp(tag, rfid_tags[i].tag) == 0) {
            log_i("RFID: tag %s detected -> %s", tag, rfid_tags[i].audio_file);
            audio_playback_start(rfid_tags[i].audio_file);
            return true;
        }
    }
    return false;
}
 

bool rfid_loop() {

    if(!rfid_scan_enable)
        return false;

    log_i("RFID Reader (Pin %d): Waiting for card...", RFID_CS); // Log that the reader is waiting for a card
    // log_i("RFID Reader (Pin %d): Waiting for card...", RFID_CS); // Log that the reader is waiting for a card
    // Check if a new card is present 
    if (!mfrc522->PICC_IsNewCardPresent()) {
        return false; // If no new card is present, exit the loop
    }; // If no new card is present, exit the loop
    
    // Attempt to read the card's serial number
    if (!mfrc522->PICC_ReadCardSerial()) {
        log_i("RFID Reader (Pin %d): Bad read (was card removed too quickly?)", RFID_CS);   
        return false;
    }

    // Check if the UID size is valid
    if (mfrc522->uid.size == 0) {
        log_i("RFID Reader (Pin %d): Bad card (size = 0)", RFID_CS);   
        
        xSemaphoreGive(spi_semaphore);
        return false;
    } 

    Uid uid = {0}; // Create a new UID object
    memcpy(&uid, &(mfrc522->uid), mfrc522->uid.size); // Copy the UID data
    char tag[20] = {0}; // Create a buffer for the tag data

    format_uid_to_hex_string(tag, &uid, sizeof(tag)); // Dump the UID data into the buffer
    
    log_i("RFID Reader (Pin %d): Good scan: %s", RFID_CS, tag); // Log the UID data

#ifdef RFID_AUTO_DISABLE_AFTER_SCAN
        rfid_scan_enable = false;   // Automatisch deaktivieren
#endif

#ifdef RFID_USE_AUDIO_TAGS
    // Check if the tag matches any predefined tags
    if (rfid_audio_tags_enable && compare_tag(tag)) { 
#if RFID_AUDIO_TAGS_DISABLE_AFTER_SCAN  
        rfid_scan_enable = false;   // Automatisch deaktivieren
#endif
#if RFID_AUDIO_TAGS_DISABLE_CALLBACK_AFTER_SCAN
        // Disengage the card
        mfrc522->PICC_HaltA();  
        mfrc522->PCD_StopCrypto1();  
        return true;
#endif
    } else {
        log_i("RFID Reader (Pin %d): No matching tag found: \"%s\"", RFID_CS, tag); // Log if no matching tag is found
    }
#endif

#ifdef RFID_USE_CALLBACK
    // Call the RFID callback if set
    if (rfid_callback) rfid_callback(tag);
#endif

    // Disengage the card
    mfrc522->PICC_HaltA();  
    mfrc522->PCD_StopCrypto1();  
     

    return true; // Return true if a card was successfully read and processed
}

void enable_rfid_scan() {
    rfid_scan_enable = true;
}

void set_rfid_audio_tags_enabled(bool enabled) {
    rfid_audio_tags_enable = enabled;
}

bool get_rfid_audio_tags_enabled() {
    return rfid_audio_tags_enable;
}

void rfid_load_tags_from_fs() {
    File dir = SD.open("/songs/rfid");
    if (!dir || !dir.isDirectory()) {
        log_e("RFID: directory /songs/rfid not found");
        return;
    }
    

    // Erst zählen
    size_t count = 0;
    File f = dir.openNextFile();
    while (f) {
        if (!f.isDirectory()) {
            String name = f.name();
            if (rfid_filename_matches_format(name)) {
                count++;
            }
        }
        f = dir.openNextFile();
    }

    dir.close();
    dir = SD.open("/songs/rfid");

    rfid_tags = new RFIDTag[count];
    rfid_tags_count = count;

    size_t idx = 0;
    f = dir.openNextFile();
    while (f) {
        if (!f.isDirectory()) {
            String name = f.name();

            if (rfid_filename_matches_format(name)) {

                String base = name.substring(name.lastIndexOf('/') + 1);
                base.replace(".mp3", "");

                rfid_tags[idx].tag = strdup(base.c_str());
                String full = String("/songs/rfid/") + base + ".mp3";
                rfid_tags[idx].audio_file = strdup(full.c_str());

                log_i("RFID: loaded tag %s -> %s",
                      rfid_tags[idx].tag,
                      rfid_tags[idx].audio_file);

                idx++;
            }
        }
        f = dir.openNextFile();
    }
}


bool rfid_filename_matches_format(const String& name) {
    if (!name.endsWith(".mp3")) return false;

    String base = name.substring(0, name.length() - 4); // ohne .mp3

    // Splitten in 4 Teile
    int parts = 0;
    int last = 0;
    int pos;

    while ((pos = base.indexOf('-', last)) != -1) {
        parts++;
        last = pos + 1;
    }
    parts++; // letzter Block

    if (parts != 4) return false;

    // Jetzt jeden Block prüfen
    last = 0;
    for (int i = 0; i < 4; i++) {
        pos = base.indexOf('-', last);
        String block = (pos == -1) ? base.substring(last) : base.substring(last, pos);

        // Muss mit 0x beginnen
        if (!block.startsWith("0x")) return false;

        if (i < 3) {
            // Die ersten 3 Blöcke müssen 0xXX sein → Länge = 4
            if (block.length() != 4) return false;

            // Hex prüfen
            char a = block[2];
            char b = block[3];
            bool isHexA = ((a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') || (a >= 'A' && a <= 'F'));
            bool isHexB = ((b >= '0' && b <= '9') || (b >= 'a' && b <= 'f') || (b >= 'A' && b <= 'F'));
            if (!isHexA || !isHexB) return false;

        } else {
            // Letzter Block MUSS exakt "0x0" sein
            if (block != "0x0") return false;
        }

        last = pos + 1;
    }

    return true;
}
