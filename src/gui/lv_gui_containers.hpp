#pragma once
#ifndef LV_GUI_CONTAINERS_HPP
#define LV_GUI_CONTAINERS_HPP

#include <utils/config.hpp>
#include <includes/includes.hpp>
#include <audio/audio.hpp>
#include <helpers/helpers.hpp>
#include <keypad/keypad.hpp>
#include <rfid/card.hpp>

#define LVBG_COLOR lv_color_hex(0x080808)
#define LVFG_COLOR lv_color_hex(0xdddddd)

 
lv_group_t* getKeypadGroup(); 
lv_indev_t* getKeypadIndevDevice();
lv_timer_t* getTimer();
void delete_timer();
void setTimer(lv_timer_t* timer);
 
/**
 * @brief Creates the Start GUI for the application.
 * 
 * This function sets up the initial graphical user interface for the application,
 * including a text label, a button, a toggle switch, a slider, and labels for the
 * switch and slider.
 * 
 * The GUI components created are:
 * - A centered text label displaying "Monopoly Banking".
 * - A button labeled "Add Card".
 * - A toggle switch with an associated label "Play 'Haut mit kein'Stein'".
 * - A slider at the bottom center of the display with a range from 0 to 21, 
 *   initially set to 18, and an animation duration of 2000 ms.
 * - A label below the slider displaying the current slider value as a percentage.
 * 
 * The function also sets up event callbacks for the toggle switch and slider.
 */
void lv_create_start_gui(void);

void remove_keyboard_and_clear_focus();
void show_keyboard_on_click(lv_event_t * event, lv_obj_t* textInput);

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// HELPER  FUNCTIONS /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
 
/**
 * @brief Asserts that the given LittlevGL object is not NULL and is valid.
 *
 * This function checks if the provided LittlevGL object pointer is NULL or invalid.
 * If the object is NULL, it logs an error message and throws a std::runtime_error.
 * If the object is not valid, it logs an error message and throws a std::runtime_error.
 * If the object is valid, it returns the object pointer.
 *
 * @param obj Pointer to the LittlevGL object to be checked.
 * @return lv_obj_t* The same object pointer if it is not NULL and valid.
 * @throws std::runtime_error if the object is NULL or not valid.
 */
lv_obj_t* lv_obj_assert_null(lv_obj_t* obj);
 
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// RFID CALLBACKS //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void clear_rfid_callback();
void enable_rfid_scan();
void set_rfid_audio_tags_enabled(bool enabled);
bool get_rfid_audio_tags_enabled();

typedef void (*RFIDCallback)(const char* uid);
void set_rfid_callback(RFIDCallback callback);
  
/**
 * @brief Times the execution of a GUI creation function asynchronously.
 * 
 * This function measures the time taken to execute a given GUI creation function asynchronously.
 * 
 * @param gui_function The GUI creation function to be timed.
 * @param param The parameter to be passed to the GUI creation function.
 */
void time_gui_creation_async(void (*gui_function)(void*), void* param);

lv_obj_t* lv_obj_assert_null(lv_obj_t* obj);

void lv_create_title_underline(lv_obj_t* parent, const char* title);
 
#endif