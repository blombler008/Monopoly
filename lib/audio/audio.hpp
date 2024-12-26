#pragma once
#ifndef AUDIO_HPP
#define AUDIO_HPP

#define AUX_MUTE HIGH
#define AUX_UNMUTE LOW

#include "includes.hpp"

/**
 * @brief Starts audio playback of the specified song.
 *
 * This function stops any currently playing audio, unmutes the audio,
 * waits for a short delay, and then connects to the specified song file
 * on the SD card to start playback.
 *
 * @param song_path The path to the song file on the SD card.
 */
void audio_playback_start(const char* song_path);

/**
 * @brief Stops audio playback.
 *
 * This function mutes the audio, stops the currently playing song, 
 * and sets the playback status to not playing.
 */
void audio_playback_stop();

/**
 * @brief Initializes the audio setup.
 *
 * This function configures the necessary pins and settings for the audio driver.
 * It sets the pin mode for the I2S mute pin, mutes the audio, configures the 
 * I2S pinout, sets the volume, and adjusts the tone settings.
 *
 * Pin Configuration:
 * - I2S_MUTE: Mute pin, set as OUTPUT.
 * - I2S_BCLK: Bit clock pin for I2S.
 * - I2S_LRC: Left-right clock pin for I2S.
 * - I2S_DOUT: Data out pin for I2S.
 *
 * Volume and Tone Settings:
 * - Volume: Set to 12.
 * - Tone: Set to -3 for bass, -3 for mid, and -12 for treble.
 *
 * Logs a message indicating that the audio driver has been initialized.
 */
void audio_setup();

/**
 * @brief Starts the audio loop task on a specific core.
 *
 * This function creates and starts a new FreeRTOS task that runs the `audioloop` function.
 * The task is pinned to core 1 and is given a stack size of 10000 bytes.
 *
 * @note Ensure that the `audioloop` function is defined elsewhere in your code.
 */
void audio_start_loop();

/**
 * @brief Sets the audio volume to the specified level.
 * 
 * This function adjusts the volume of the audio output to the given level.
 * 
 * @param vol The desired volume level, specified as an 8-bit unsigned integer.
 *            The valid range of values depends on the implementation of the 
 *            audio library being used. Typically, the range is from 0 to 21.
 * 
 * @note The actual volume level may vary depending on the hardware and the
 *       audio library being used. The volume level may be adjusted in discrete
 *       steps or may have a non-linear relationship with the input value.
 * 
 * @note The default volume level is set to a value of 21.
 */
void audio_set_volume(uint8_t vol = 21);

/**
 * @brief Mutes or unmutes the audio output.
 * 
 * This function controls the mute state of the audio output by setting the 
 * appropriate digital pin to either mute or unmute the audio.
 * 
 * @param mute A boolean value indicating whether to mute (true) or unmute (false) the audio.
 */
void audio_mute(bool mute = true);

/**
 * @brief Checks if the audio is currently playing.
 * 
 * @return true if the audio is playing, false otherwise.
 */
bool audio_is_playing();

/**
 * @brief Recursively finds all audio files with the .mp3 extension in the specified directory and its subdirectories up to a specified maximum depth.
 * 
 * @param audioFiles An array of Strings to store the names of the found audio files.
 * @param fileCount A reference to an integer that keeps track of the number of found audio files.
 * @param directory The path to the directory where the search should begin.
 * @param maxDepth The maximum depth of subdirectories to search.
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
char** findAudioFiles(int& fileCount, const char* directory = "/audio/", int maxDepth = 1);

#endif