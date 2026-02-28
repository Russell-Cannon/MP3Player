#include "pico/stdlib.h"
#include "lib/lvgl/lvgl.h"
#include "st7789.h"
#include <stdio.h>

void lvgl_driver_init(void);

bool lvgl_tick_cb(struct repeating_timer *t) {
    lv_tick_inc(5);
    return true;
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000); // give USB serial time to connect
    printf("Starting...\n");
    
    st7789_init();
    printf("Display init done\n");
    
    lv_init();
    printf("LVGL init done\n");
    
    lvgl_driver_init();
    printf("Driver init done\n");
    struct repeating_timer timer;
    add_repeating_timer_ms(5, lvgl_tick_cb, NULL, &timer);

    // --- UI ---
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x003a57), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Hello, Oscar!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (true) {
        lv_timer_handler();
        sleep_ms(5);
    }
}