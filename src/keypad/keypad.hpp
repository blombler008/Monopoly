#pragma once
#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <includes/includes.hpp>
#include <utils/macros.hpp>
#include <utils/config.hpp>
typedef void (*KEYPADCallback)(char);
void keypad_set_address(uint8_t address);
void keypad_set_layout(const char* layout);
void keypad_setup(void (*KEYPADCallback)(char) = NULL);
void keypad_stop(); 
I2CKeyPad getKeypad(void);

#endif