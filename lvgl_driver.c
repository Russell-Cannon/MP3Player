#include "lib/lvgl/lvgl.h"
#include "st7789.h"

// Draw buffer: two buffers of 1/10th screen height for double-buffering
#define DRAW_BUF_LINES 24
static lv_color_t buf1[ST7789_WIDTH * DRAW_BUF_LINES];
static lv_color_t buf2[ST7789_WIDTH * DRAW_BUF_LINES];

static lv_display_t *disp;

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    st7789_set_window(area->x1, area->y1, area->x2, area->y2);
    st7789_write_pixels((uint16_t *)px_map, w * h);

    lv_display_flush_ready(disp);
}

void lvgl_driver_init(void) {
    disp = lv_display_create(ST7789_WIDTH, ST7789_HEIGHT);
    lv_display_set_flush_cb(disp, disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}