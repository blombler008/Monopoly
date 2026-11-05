#include "sd_manager.hpp"

#include "main.hpp"

static SPIClass* s_spi = nullptr;
static bool s_sdAvailable = false;

void sd_led_update(bool sdAvailable);

static bool sd_detect() {
    return digitalRead(SD_DETECT_PIN) == HIGH;
}

static void sd_handle_insert() {
    if (!SD.begin(SD_CS, *s_spi, SD_SPI_SPEED)) {
        log_e("Failed to initialize SD card");
        return;
    }

    show_sd_info(false);

    s_sdAvailable = true;

    fire_event(EventId::SDCardDetected);
}

static void sd_handle_remove() {
    SD.end();

    s_sdAvailable = false;

    fire_event(EventId::SDCardRemoved);
}

void sd_setup(SPIClass* spi) {
    s_spi = spi;

    pinMode(SD_STATUS_LED, OUTPUT);
    digitalWrite(SD_STATUS_LED, LOW);

    pinMode(SD_DETECT_PIN, INPUT_PULLUP);

    digitalWrite(SD_STATUS_LED, HIGH);

    s_sdAvailable = sd_detect();

    if (!s_sdAvailable)
        return;

    if (!SD.begin(SD_CS, *s_spi, SD_SPI_SPEED)) {
        log_e("SD Card initialization failed!");
        digitalWrite(SD_STATUS_LED, LOW);
        s_sdAvailable = false;
        return;
    }

    log_i("SD Card initialized successfully!");

    coreSettings.load();

    fire_event(EventId::SDCardDetected);

    show_sd_info();
}

void sd_update() {
    bool cardPresent = sd_detect();

    if (cardPresent == s_sdAvailable) {
        sd_led_update(s_sdAvailable);
        return;
    }

    if (cardPresent)
        sd_handle_insert();
    else
        sd_handle_remove();

    sd_led_update(s_sdAvailable);
}

bool sd_available() {
    return s_sdAvailable;
}