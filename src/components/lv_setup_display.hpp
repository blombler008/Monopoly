/**
 * @file lv_setup_display.hpp
 * @brief Header file for setting up the display using LittlevGL (LVGL) library.
 *
 * This file contains the necessary includes, macro definitions, and function
 * declarations required to initialize and configure the display for the
 * Monopoly project.
 *
 * Macros:
 * - TFT_FONT: Specifies the font to be used for the display.
 * - DRAW_BUF_SIZE: Defines the size of the draw buffer based on the screen
 *   dimensions and color depth.
 *
 * Functions:
 * - void lv_setup_display(void): Initializes and sets up the display.
 */
#ifndef LV_SETUP_DISPLAY
#define LV_SETUP_DISPLAY

#include <utils/config.hpp>
#include <includes/includes.hpp>
#include "lv_gui_containers.hpp"

#define TFT_FONT &lv_font_montserrat_14_n
#define DRAW_BUF_SIZE (TFT_SCREEN_WIDTH * TFT_SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))


/**
 * @brief Initializes and sets up the LVGL display and touch input.
 * 
 * This function performs the following tasks:
 * - Constructs a version string for LVGL and prints it to the serial output.
 * - Initializes the LVGL library.
 * - Sets the tick callback function for LVGL.
 * - Creates a display buffer and initializes the display driver.
 * - Sets the display rotation.
 * - Calibrates the touch screen with predefined calibration data.
 * - Creates an input device for touch input and sets the read callback function.
 */
void lv_setup_display(void);
void lvgl_print_version(void);

#endif