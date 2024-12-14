#ifndef MAIN_HPP
#define MAIN_HPP

#include "includes.hpp"
#include "custom_includes.hpp"
#include "card.hpp"

class helper {
    public:
    
        /**
         * @brief Dumps the UID byte array into a formatted string.
         *
         * This function takes a UID byte array from an MFRC522 RFID reader and 
         * formats it into a human-readable hexadecimal string with each byte 
         * separated by a dash. The formatted string is appended to the provided 
         * tag buffer.
         *
         * @param tag A pointer to the character buffer where the formatted UID 
         *            string will be stored.
         * @param uid A pointer to the MFRC522::Uid structure containing the UID 
         *            byte array.
         * @param tag_size The size of the tag buffer to ensure no buffer overflow 
         *                 occurs.
         */
        static void dump_byte_array(char* tag, MFRC522::Uid* uid, size_t tag_size);
        
        /**
         * @brief Handles keypad events and triggers corresponding actions.
         * 
         * This function is called whenever a keypad event occurs. It checks the state 
         * of the key and performs actions based on the key pressed. Specifically:
         * - If the key '1' is pressed, it starts audio playback of "intro.mp3".
         * - If the key '2' is pressed, it starts audio playback of "Haut.mp3".
         * - If the key '*' is pressed, it stops the audio playback.
         * 
         * @param key The key that triggered the event.
         */
        static void keypadEvent(KeypadEvent key);
}; 


/**
 * @brief Initializes the hardware and software components for the Monopoly project.
 * 
 * This function is responsible for setting up the serial communication, initializing 
 * various peripherals, and starting the main application loop. The setup process includes:
 * 
 * 1. Initializing the serial communication with a predefined monitor speed for debugging purposes.
 * 2. Initializing two SPI buses (VSPI and HSPI) for communication with different peripherals.
 * 3. Setting up the display using the LittlevGL (lvgl) library, which includes configuring 
 *    the display driver and creating the initial graphical user interface.
 * 4. Configuring the keypad by setting the number of rows and columns, assigning the pin 
 *    connections, and defining the keypad layout. It also sets up the keypad event handler.
 * 5. Initializing the SD card with a specified chip select pin and setting up the audio 
 *    system if the SD card initialization is successful. This includes starting the audio 
 *    loop for continuous audio playback.
 * 6. Initializing multiple MFRC522 RFID readers by iterating through the defined number of 
 *    readers, initializing each reader, and performing a version dump to the serial monitor 
 *    for debugging. The code also includes commented-out sections for performing self-tests 
 *    on the RFID readers.
 * 
 * The function concludes by logging the start of the main application loop. This setup 
 * function is crucial for preparing the hardware and software environment before entering 
 * the main operational loop of the application.
 */
void setup();



/**
 * @brief Main loop function that continuously checks for new RFID cards on multiple readers.
 * 
 * This function iterates over a predefined number of RFID readers, checking each one for the presence of a new card.
 * If a new card is detected, it attempts to read the card's serial number and logs the result.
 * The card is then disengaged to allow for subsequent reads.
 * 
 * @note The function assumes that the array `mfrc522` and `ssPins` are properly initialized and accessible.
 * 
 * @details
 * - The function iterates over the readers using a for loop.
 * - For each reader, it checks if a new card is present using `PICC_IsNewCardPresent()`.
 * - If a new card is detected, it reads the card's serial number using `PICC_ReadCardSerial()`.
 * - If the read is successful and the card's UID size is valid, it logs the UID.
 * - The card is then disengaged using `PICC_HaltA()` and `PCD_StopCrypto1()`.
 * 
 * @warning Ensure that `NR_OF_READERS` is correctly defined and matches the number of readers connected.
 * @warning The function may need to be modified to handle additional error cases or edge conditions.
 */
void loop();

#endif