#include "helpers.hpp"

void helper::format_uid_to_hex_string(char* tag, MFRC522::Uid* uid, size_t tag_size) { 
    memset(tag, 0, tag_size); // Clear the tag buffer to ensure it is empty before appending data, and prevent buffer overflow

    if(uid->size == 0) { // Check if the UID size is zero
        strncat(tag, "0x0", tag_size - strlen(tag) - 1); // Append "0x0" to the tag buffer
        return; // Exit the function
    }

    if(tag_size < 20) { // Check if the tag buffer is too small
        log_e("Tag buffer too small"); // Log an error message
        return; // Exit the function
    }

    if(uid->size > 10) { // Check if the UID size is too large
        log_e("UID size too large"); // Log an error message
        return; // Exit the function
    }

    if((uid->size * 5) > (tag_size + 1)) { // Check if the UID size is too large for the tag buffer
        log_e("UID size too large for tag buffer"); // Log an error message
        return; // Exit the function
    }

    for (int j = 0; j < uid->size; j++) { // Iterate over each byte in the UID
        char buff[7]; // 3 digits, dashes, a letter and \0 is 7 characters at most. 6+1=7. 1 extra here for safety.
        snprintf(buff, sizeof(buff), "%s%x", j ? "-0x" : "0x", uid->uidByte[j]); // Print one byte of the UID in hexadecimal format
        strncat(tag, buff, tag_size - strlen(tag) - 1); // Append the byte to the tag buffer
    };
     
}