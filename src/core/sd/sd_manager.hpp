#ifndef SD_MANAGER_HPP
#define SD_MANAGER_HPP

#include <Arduino.h>
#include <SPI.h>

void sd_setup(SPIClass* spi);
void sd_update();

bool sd_available();

#endif