/**
 * @file lv_tft_espi.h
 *
 */

#ifndef LV_TFT_ESPI_H
#define LV_TFT_ESPI_H

#ifdef __cplusplus
#include <SPI.h>
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../display/lv_display.h"

#if LV_USE_TFT_ESPI

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#ifdef __cplusplus
lv_display_t * lv_tft_espi_create(uint32_t hor_res, uint32_t ver_res, void * buf1, void * buf2, uint32_t buf_size_bytes, SPIClass* _SPI);
#endif

/**********************
 *      MACROS
 **********************/

#endif /* LV_USE_TFT_ESPI */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_TFT_ESPI_H */
