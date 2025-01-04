#pragma once
#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <includes/includes.hpp>
 
void keypad_set_pins(byte* rowPins, byte* colPins);
void keypad_set_row_col_num(const byte rows, const byte cols);
void keypad_set_layout(char* layout);
void keypad_setup(void (*listener)(char) = NULL);
void keypad_stop();
int getKeystateString(char* buf, char key);
Keypad getKeypad(void);

#endif