#pragma once
#ifndef HELPERS_HPP
#define HELPERS_HPP
#include <MFRC522.h>
using Uid = MFRC522::Uid;
extern SemaphoreHandle_t spi_semaphore;
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
void format_uid_to_hex_string(char* tag, Uid* uid, size_t tag_size);

/**
 * @brief Führt eine SPI-Operation mutexgeschützt aus.
 * 
 * @param label Bezeichner für Logging (z. B. "Touch", "RFID", "Display")
 * @param timeout_ms Timeout in Millisekunden (z. B. 10)
 * @param fn Lambda oder Funktion, die ausgeführt wird, wenn der Mutex verfügbar ist.
 * @return true wenn erfolgreich ausgeführt, false wenn Timeout oder Fehler
 */
bool SPI_guarded(const char* label, uint32_t timeout_ms, std::function<void()> fn);

#endif