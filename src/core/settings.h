#pragma once
#include <ArduinoJson.h>
#include <SD.h>
#include <string>

struct EconomySettings {
    int startCash = 1500;
    int passGoCash = 200;
    bool allowCredit = true;
    int taxFixedAmount = 100;
    std::string currencyChar = "M";
};

struct RuleSettings {
    uint8_t maxPlayers = 8;
    bool mortgagesAllowed = true;
    bool auctionsEnabled = false;
    bool doubleDiceBonus = false;
    bool freeParkingJackpot = false;
    bool jackpotEnabled = false;
};

struct SystemSettings {
    bool soundEnabled = true;
    uint8_t volume = 80;
    uint8_t autosaveIntervalSec = 30;
    std::string language = "de";
    std::string adminPin = "9904";
};

class Settings {
public:
    EconomySettings economy;
    RuleSettings rules;
    SystemSettings system;

    bool load();
    void save();
    void autosave();

private:
    bool dirty = false;
    void markDirty() { dirty = true; }
};
