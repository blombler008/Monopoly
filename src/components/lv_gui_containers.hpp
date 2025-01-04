#pragma once
#ifndef LV_GUI_CONTAINERS_HPP
#define LV_GUI_CONTAINERS_HPP

#include <utils/config.hpp>
#include <includes/includes.hpp>
#include <settings/settings.hpp>
#include <audio/audio.hpp>
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
 * @brief Starts the main loop for the LVGL (Light and Versatile Graphics Library) GUI.
 * 
 * This function initializes and starts the main loop required for the LVGL library to
 * handle GUI updates and events. It should be called once during the setup phase of
 * the application to ensure the GUI operates correctly.
 */
void lv_start_loop(void);
void lv_stop_loop(void);

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
void lv_create_title_underline(lv_obj_t* parent, const char* title);

void clear_rfid_callback();

typedef void (*RFIDCallback)(const char* uid);
void set_rfid_callback(RFIDCallback callback);

void lv_create_add_card_gui(const char* card_name);
void lv_create_card_found_gui(Card* card);
void lv_create_card_edit_gui(bool readOnly);
void lv_create_remove_card_gui(Card* card);
void lv_create_cards_gui(void);
void lv_create_transfer_gui(void);
void lv_create_transfer_confirm_gui(void);
void lv_create_settings_gui(void);
void lv_create_admin_card_gui(void);
void lv_create_player_card_gui(void);

enum GuiScreens {
    START_GUI,
    ADD_CARD_GUI,
    CARD_FOUND_GUI,
    CARD_EDIT_GUI,
    REMOVE_CARD_GUI,
    CARDS_GUI,
    TRANSFER_GUI,
    TRANSFER_CONFIRM_GUI,
    SETTINGS_GUI,
    ADMIN_CARD_GUI,
    PLAYER_CARD_GUI
};

/**
 * @brief Switches the current GUI screen to the specified screen.
 * 
 * This function handles the transition between different GUI screens in the application.
 * It ensures that the appropriate GUI setup function is called based on the specified
 * screen identifier.
 * 
 * @param screen The identifier of the screen to switch to.
 * @param user_data Optional user data to pass to the GUI setup function.
 */
void lv_screen_switch(GuiScreens screen, void* user_data = nullptr);
 
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