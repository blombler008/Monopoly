#include "settings.hpp"

GameSettings defaultSettings = {
    .maxPlayers = 4,             // maxPlayers
    .startingMoney = 1500,       // startingMoney 
    .currency = (char*)"M"       // currency
};

void setMaxPlayers(int maxPlayers) {
    if (maxPlayers > 0 && maxPlayers <= 8) {
        defaultSettings.maxPlayers = maxPlayers;
    }
}

void setStartingMoney(int startingMoney) {
    if (startingMoney >= 0) {
        defaultSettings.startingMoney = startingMoney;
    }
} 

void setCurrency(const char* currency) {

    if(currency == NULL) { 
        throw std::invalid_argument("Currency is NULL");
        return;
    }

    strcpy(defaultSettings.currency, currency); 
}

int getMaxPlayers() {
    return defaultSettings.maxPlayers;
}

int getStartingMoney() {
    return defaultSettings.startingMoney;
}
 
const char* getCurrency() {
    return defaultSettings.currency;
}