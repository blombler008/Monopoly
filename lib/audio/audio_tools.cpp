#include "audio.hpp"
/**
 * @brief Recursively finds all audio files with the .mp3 extension in the specified directory and its subdirectories up to a specified maximum depth.
 * 
 * @param audioFiles An array of Strings to store the names of the found audio files.
 * @param fileCount A reference to an integer that keeps track of the number of found audio files.
 * @param directory The path to the directory where the search should begin.
 * @param maxDepth The maximum depth of subdirectories to search.
 * @param currentDepth The current depth of the search (used internally for recursion).
 * 
 * This function initializes the fileCount to 0 on the first call and sets a flag to prevent reinitialization on subsequent recursive calls.
 * It opens the specified directory and iterates through its contents. If a subdirectory is found and the current depth is less than maxDepth, the function calls itself recursively.
 * If an .mp3 file is found, its name is added to the audioFiles array, and the fileCount is incremented.
 * 
 * If the directory cannot be opened or is not a directory, an error message is logged.
 * The isFirstCall flag is reset to true when the initial directory is "/".
 * 
 * @warning The function is not thread-safe and should not be called from multiple threads simultaneously.
 * @warning The function assumes that the SD card is mounted and accessible.
 * @warning The function is not optimized for performance and may be slow for large directory structures.
 * @warning The function may not handle all edge cases and error conditions.
 * @warning The function may not be suitable for real-time or time-critical applications.
 * @warning The function is for internal use only and may not be used outside of the environment.
 * 
 */
char** findAudioFiles(int& fileCount, const char* directory, int maxDepth, int currentDepth = 0) {
    static bool isFirstCall = true; // Flag to check if it is the first call
    static char** audioFiles = nullptr; // Array to store the file paths
    static int capacity = 10; // Initial capacity of 10 files
    if (isFirstCall) { // Initialize on first call
        fileCount = 0; // Initialize file count to 0
        isFirstCall = false; // Reset the flag
        audioFiles = new char*[capacity]; // Allocate memory for the array of file paths
    }
    
    if (currentDepth > maxDepth) { // Check for maxDepth reached
        return audioFiles;
    }

    if(maxDepth < 0) { // Check for negative maxDepth
        log_e("Invalid maxDepth");
        return audioFiles;
    }

    if(directory == nullptr) { // Check for null directory
        log_e("Invalid arguments");
        return audioFiles;
    }

    File root = SD.open(directory);
    if (!root) { // Check if directory can be opened
        log_e("Failed to open directory");
        return audioFiles;
    }

    if (!root.isDirectory()) { // Check if directory is a directory
        log_e("Not a directory");
        return audioFiles;
    }

    File file = root.openNextFile(); // Get first file
    while (file) { // Iterate through directory contents

        if (file.isDirectory()) { // Recursively search subdirectories
            findAudioFiles(fileCount, file.name(), maxDepth, currentDepth + 1); // Search subdirectory and increment currentDepth
            file = root.openNextFile(); // Get next file
            continue;
        } 
        
        String fileName = file.name(); // Get file name

        if (!fileName.endsWith(".mp3")) { // Check for .mp3 extension
            file = root.openNextFile(); // Get next file
            continue;
        }

        if (fileCount >= capacity) { // Resize the array if capacity is reached
            // Resize the array if capacity is reached
            capacity *= 2; // Double the capacity
            char** newAudioFiles = new char*[capacity]; // Create a new array with the new capacity
            for (int i = 0; i < fileCount; ++i) { // Copy the existing elements to the new array
                newAudioFiles[i] = audioFiles[i]; // Copy the pointers
            }
            delete[] audioFiles; // Delete the old array
            audioFiles = newAudioFiles; // Update the pointer to the new array
        }

        String fullPath = String(directory) + "/" + fileName; // Get the full path of the file
        audioFiles[fileCount] = new char[fullPath.length() + 1]; // Allocate memory for the file path
        strcpy(audioFiles[fileCount], fullPath.c_str()); // Copy the file path to the array
        fileCount++; // Increment the file count
        
        file = root.openNextFile(); // Get next file
    }
    if (directory == "/") {
        isFirstCall = true;
    }
    return audioFiles;
}

char** findAudioFiles(int& fileCount, const char* directory, int maxDepth) {
    return findAudioFiles(fileCount, directory, maxDepth, 0); // Start with currentDepth = 0
}