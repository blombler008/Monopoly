#include "Settings.h"

#define SETTINGS_PATH "/monopoly/settings.json"
#define SETTINGS_TMP  "/monopoly/settings.tmp"
#define SCHEMA_VERSION 1

// ------------------------------------------------------------
// Load
// ------------------------------------------------------------
bool Settings::load()
{
    if (!SD.exists("/monopoly")) {
        SD.mkdir("/monopoly");
    }

    if (!SD.exists(SETTINGS_PATH)) {
        save();  // Erstmal Default speichern
        return true;
    }

    File file = SD.open(SETTINGS_PATH, FILE_READ);
    if (!file) return false;

    DynamicJsonDocument doc(8192);

    DeserializationError err = deserializeJson(doc, file);
    file.close();

    if (err) return false;

    uint8_t schema = doc["schema_version"] | 1;
    if (schema != SCHEMA_VERSION) {
        // Hier könnte später Migration stattfinden
    }

    // -------- Economy --------
    economy.startCash      = doc["economy"]["start_cash"] | 1500;
    economy.passGoCash     = doc["economy"]["pass_go_cash"] | 200;
    economy.allowCredit    = doc["economy"]["allow_credit"] | true;
    economy.taxFixedAmount = doc["economy"]["tax_fixed_amount"] | 100;
    economy.currencyChar   = doc["economy"]["currency_char"] | "M";

    // -------- Rules --------
    rules.maxPlayers           = doc["rules"]["max_players"] | 8;
    rules.mortgagesAllowed     = doc["rules"]["mortgages_allowed"] | true;
    rules.auctionsEnabled      = doc["rules"]["auctions_enabled"] | false;
    rules.doubleDiceBonus      = doc["rules"]["double_dice_bonus"] | false;
    rules.freeParkingJackpot   = doc["rules"]["free_parking_jackpot"] | false;
    rules.jackpotEnabled       = doc["rules"]["jackpot_enabled"] | false;

    // -------- System --------
    system.soundEnabled        = doc["system"]["sound_enabled"] | true;
    system.volume              = doc["system"]["volume"] | 80;
    system.autosaveIntervalSec = doc["system"]["autosave_interval_sec"] | 30;
    system.language            = doc["system"]["language"] | "de";
    system.adminPin            = doc["system"]["admin_pin"] | "9904";

    dirty = false;
    return true;
}


// ------------------------------------------------------------
// Save (Atomic)
// ------------------------------------------------------------
void Settings::save()
{
    DynamicJsonDocument doc(8192);

    doc["schema_version"] = SCHEMA_VERSION;

    // -------- Economy --------
    doc["economy"]["start_cash"]       = economy.startCash;
    doc["economy"]["pass_go_cash"]     = economy.passGoCash;
    doc["economy"]["allow_credit"]     = economy.allowCredit;
    doc["economy"]["tax_fixed_amount"] = economy.taxFixedAmount;
    doc["economy"]["currency_char"]    = economy.currencyChar;

    // -------- Rules --------
    doc["rules"]["max_players"]            = rules.maxPlayers;
    doc["rules"]["mortgages_allowed"]      = rules.mortgagesAllowed;
    doc["rules"]["auctions_enabled"]       = rules.auctionsEnabled;
    doc["rules"]["double_dice_bonus"]      = rules.doubleDiceBonus;
    doc["rules"]["free_parking_jackpot"]   = rules.freeParkingJackpot;
    doc["rules"]["jackpot_enabled"]        = rules.jackpotEnabled;

    // -------- System --------
    doc["system"]["sound_enabled"]         = system.soundEnabled;
    doc["system"]["volume"]                = system.volume;
    doc["system"]["autosave_interval_sec"] = system.autosaveIntervalSec;
    doc["system"]["language"]              = system.language;
    doc["system"]["admin_pin"]             = system.adminPin;

    File tmp = SD.open(SETTINGS_TMP, FILE_WRITE);
    if (!tmp) return;

    serializeJsonPretty(doc, tmp);
    tmp.close();

    SD.remove(SETTINGS_PATH);
    SD.rename(SETTINGS_TMP, SETTINGS_PATH);

    dirty = false;
}


// ------------------------------------------------------------
// Autosave
// ------------------------------------------------------------
void Settings::autosave()
{
    if (dirty) {
        save();
    }
}
