#include "serial_commands_sd.hpp"

#include <string.h>
#include <Arduino.h>
#include <SD.h>
#include "core/event/event_ids.hpp"
#include "core/event/event_manager.hpp"
#include "serial_registry.hpp"

static const SerialCommand SD_COMMAND = {
    "sd",
    "SD management",
    serial_sd_execute,
    serial_sd_help
};

void register_sd_commands() {
    serial_register_command(&SD_COMMAND);
}

static void serial_sd_help() {
    log_i("");
    log_i("SD commands:");
    log_i("");
    log_i("sd detect");
    log_i("sd remove");
    log_i("sd reload");
    log_i("sd info");
    log_i("sd ls [path]");
    log_i("sd tree [path]");
    log_i("sd exists <path>");
    log_i("");
}

bool serial_sd_execute(const char* command) {
    char path[256];

    if(strcmp(command, "info") == 0) {
        show_sd_info();
        return true;
    }

    if (strcmp(command, "reload") == 0){
        fire_event(EventId::SDCardRemoved);

        vTaskDelay(pdMS_TO_TICKS(20));

        fire_event(EventId::SDCardDetected);

        return true;
    }

    if (strcmp(command, "detect") == 0) {
        fire_event(EventId::SDCardDetected);
        return true;
    }

    if (strcmp(command, "remove") == 0) {
        fire_event(EventId::SDCardRemoved);
        return true;
    }
  
    if(sscanf(command, "ls %255[^\n]", path) == 1) {
        sd_list_directory(path);
        return true;
    }

    if(strcmp(command, "ls") == 0) {
        sd_list_directory("/");
        return true;
    }

    if(sscanf(command, "tree %255[^\n]", path) == 1) {
        sd_tree(path);
        return true;
    }

    if(strcmp(command, "tree") == 0) {
        sd_tree("/");
        return true;
    }

    if(sscanf(command, "exists %255[^\n]", path) == 1) {
        sd_exists(path);
        return true;
    }

    return false;
}

void make_bargraph(int numChars, float usedPercentage, char* label) {
    int num_hashes = static_cast<int>(usedPercentage / 2); 
    num_hashes = std::min(num_hashes, numChars);  
    std::string bar(num_hashes, '#'); 
    bar.append(numChars - num_hashes, ' ');  
    strcpy(label, bar.c_str()); 
}

void show_sd_info(bool more) {
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

static void sd_list_directory(const char* path) {
    File dir = SD.open(path);

    if (!dir || !dir.isDirectory()) {
        log_e("Directory not found: %s", path);
        return;
    }

    File file = dir.openNextFile();

    while (file) {
        log_i(
            "%c %8u %s",
            file.isDirectory() ? 'D' : 'F',
            file.size(),
            file.name());

        file = dir.openNextFile();
    }
}

static void sd_tree(File dir, uint8_t level) {
    File file = dir.openNextFile();

    while(file) {
        for(uint8_t i = 0;i < level;i++) {
            Serial.print("  ");
        }

        log_i( "%s%s", file.isDirectory() ? "[DIR] " : "", file.name());

        if(file.isDirectory()) {
            sd_tree(file, level + 1);
        }

        file = dir.openNextFile();
    }
}

static void sd_tree(const char* path) { 
    File dir = SD.open(path);

    if(!dir || !dir.isDirectory()) {
        log_e( "Directory not found: %s", path);
        return;
    }

    sd_tree(dir, 0);
}

static void sd_exists(const char* path) {
    log_i("%s : %s", path, SD.exists(path) ? "YES" : "NO");
}
