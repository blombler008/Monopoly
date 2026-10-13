/**
 * @file lv_tft_espi.cpp
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl_public.h"
#if LV_USE_TFT_ESPI

#include <TFT_eSPI.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    TFT_eSPI * tft;
} lv_tft_espi_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
static void resolution_changed_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_display_t * lv_tft_espi_create(uint32_t hor_res, uint32_t ver_res, void * buf1, void * buf2, uint32_t buf_size_bytes)
{
    LV_CHECK_ARG(buf1 != NULL, return NULL);
    LV_CHECK_ARG(buf2 != NULL, return NULL);
    lv_tft_espi_t * dsc = (lv_tft_espi_t *)lv_malloc_zeroed(sizeof(lv_tft_espi_t));
    LV_ASSERT_MALLOC(dsc);
    if(dsc == NULL) return NULL;

    lv_display_t * disp = lv_display_create(hor_res, ver_res);
    if(disp == NULL) {
        lv_free(dsc);
        return NULL;
    }

    dsc->tft = new TFT_eSPI(ver_res, hor_res);
    dsc->tft->init();          /* TFT init */
    dsc->tft->setRotation(2);
    // dsc->tft->initDMA(true);          /* TFT init */
    lv_display_set_driver_data(disp, (void *)dsc);
    lv_display_set_flush_cb(disp, flush_cb);
    lv_display_add_event_cb(disp, resolution_changed_event_cb, LV_EVENT_RESOLUTION_CHANGED, NULL);
    lv_display_set_buffers(disp, (void *)buf1, (void *)buf2, buf_size_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL);
    return disp;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    // make a buffer copy of the pixel data to avoid modifying the original buffer
    uint8_t * buffer_copy = (uint8_t *)lv_malloc_zeroed((area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * sizeof(uint16_t));
    memcpy(buffer_copy, px_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * sizeof(uint16_t));
  
    lv_tft_espi_t * dsc = (lv_tft_espi_t *)lv_display_get_driver_data(disp);

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    dsc->tft->startWrite();
    dsc->tft->setAddrWindow(area->x1, area->y1, w, h);
    // log_i("area=(%d,%d)-(%d,%d), w=%d, h=%d, px_map=%p", area->x1, area->y1, area->x2, area->y2, w, h, buffer_copy);

    // buffer dump : 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F
    // 32 bytes per line, 2 bytes per pixel (RGB565)
    // log_i("Buffer dump:");
    // char * buffer = (char *)buffer_copy;
    // for (uint32_t i = 0; i < w * h * 2; i += 32) {
    //     char line[160];
    //     int len = snprintf(line, sizeof(line), "%04X: ", i);
    //     for (uint32_t j = 0; j < 32 && (i + j) < w * h * 2; j++) {
    //         len += snprintf(line + len, sizeof(line) - len, "%02X ", (unsigned char)buffer[i + j]);
    //     }
    //     log_i("%s", line);
    // }
    dsc->tft->pushImage(area->x1, area->y1, w, h, (uint16_t *)buffer_copy);


    // dsc->tft->pushColors((uint16_t *)px_map, w * h, true);
    // dsc->tft->pushPixelsDMA((uint16_t*)px_map, w * h);  
    dsc->tft->endWrite();
    lv_display_flush_ready(disp);
}

static void resolution_changed_event_cb(lv_event_t * e)
{
    lv_display_t * disp = (lv_display_t *)lv_event_get_target(e);
    lv_tft_espi_t * dsc = (lv_tft_espi_t *)lv_display_get_driver_data(disp);
    int32_t hor_res = lv_display_get_horizontal_resolution(disp);
    int32_t ver_res = lv_display_get_vertical_resolution(disp);
    lv_display_rotation_t rot = lv_display_get_rotation(disp);

    /* handle rotation */
    switch(rot) {
        case LV_DISPLAY_ROTATION_0:
            dsc->tft->setRotation(1);   /* Portrait orientation */
            break;
        case LV_DISPLAY_ROTATION_90:
            dsc->tft->setRotation(2);   /* Landscape orientation */
            break;
        case LV_DISPLAY_ROTATION_180:
            dsc->tft->setRotation(3);   /* Portrait orientation, flipped */
            break;
        case LV_DISPLAY_ROTATION_270:
            dsc->tft->setRotation(0);   /* Landscape orientation, flipped */
            break;
    }
}

#endif /*LV_USE_TFT_ESPI*/
