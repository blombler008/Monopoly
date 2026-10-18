/**
 * @file lv_tft_espi.h
 *
 */

#ifndef LV_TFT_ESPI_CUSTOM_H
#define LV_TFT_ESPI_CUSTOM_H
 
#include <SPI.h> 
/*********************
 *      INCLUDES
 *********************/
#include "include/lvgl/display/lv_display.h"

 
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/ 
lv_display_t * lv_tft_espi_create(uint32_t hor_res, uint32_t ver_res, void * buf1, void * buf2, uint32_t buf_size_bytes, SPIClass* _SPI); 
/**********************
 *      MACROS
 **********************/
   
#endif /* LV_TFT_ESPI_CUSTOM_H */
