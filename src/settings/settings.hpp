#pragma once
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <includes/includes.hpp>

struct GameSettings {
    uint8_t maxPlayers;
    int32_t startingMoney; 
    char* currency;
};

extern GameSettings defaultSettings;

void setMaxPlayers(int maxPlayers);
void setStartingMoney(int startingMoney); 
void setCurrency(const char* currency);

int getMaxPlayers();
int getStartingMoney(); 
const char* getCurrency();

#endif // SETTINGS_HPP