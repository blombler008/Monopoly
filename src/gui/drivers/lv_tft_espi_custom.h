/**
 * @file lv_tft_espi.h
 *
 */

#ifndef LV_TFT_ESPI_CUSTOM_H
#define LV_TFT_ESPI_CUSTOM_H
 
#include <SPI.h>  
#include "include/lvgl/display/lv_display.h"
#include "lvgl_public.h" 
#include <helpers/helpers.hpp>
#include <TFT_eSPI.h>
 

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/ 
lv_display_t * lv_tft_espi_create(uint32_t hor_res, uint32_t ver_res, void * buf1, void * buf2, uint32_t buf_size_bytes, SPIClass* _SPI); 
IRAM_ATTR void notify_lvgl_flush_ready(void *user_ctx);
static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
static void resolution_changed_event_cb(lv_event_t * e);
/**********************
 *      MACROS
 **********************/
   
#endif /* LV_TFT_ESPI_CUSTOM_H */
