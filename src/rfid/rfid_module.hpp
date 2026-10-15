#ifndef RFID_MODULE_HPP
#define RFID_MODULE_HPP 

#include <includes/includes.hpp>
#include <utils/macros.hpp>
#include <helpers/helpers.hpp>
#include <utils/config.hpp>
#include <gui/lv_gui_containers.hpp>


  
void rfid_setup(SPIClass* v_spi = &SPI);
bool rfid_loop();


#endif // RFID_MODULE_HPP