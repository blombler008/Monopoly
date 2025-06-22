#ifndef MAIN_HPP
#define MAIN_HPP
 

#include <utils/config.hpp> 
#include <includes/includes.hpp>

#include <utils/macros.hpp>
#include <utils/constants.hpp>
#include <includes/custom_includes.hpp>
#include <helpers/helpers.hpp>
 
using Uid = MFRC522::Uid;

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
void setupMain();

#endif