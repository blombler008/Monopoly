#pragma once
#ifndef HELPERS_HPP
#define HELPERS_HPP
#include <MFRC522.h>

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
        static void format_uid_to_hex_string(char* tag, MFRC522::Uid* uid, size_t tag_size);
}; 

#endif