#include <lvgl.h>
#include <TFT_eSPI.h>
#include "lvgl_ui.h"

TFT_eSPI tft = TFT_eSPI();
static lv_obj_t *label_user;
static lv_obj_t *label_ai;
static lv_obj_t *label_status;
static lv_obj_t *todo_list;

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

// === LVGL flush callback ===
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// === Init LVGL UI ===
void ui_init() {
    lv_init();
    tft.begin();
    tft.setRotation(1);

    static lv_color_t buf1[SCREEN_WIDTH * 10];
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, SCREEN_WIDTH * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Create base screen
    lv_obj_t *scr = lv_scr_act();

    // Header
    lv_obj_t *header = lv_label_create(scr);
    lv_label_set_text(header, "🔵 Jarvis Assistant");
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 5);

    // User text
    label_user = lv_label_create(scr);
    lv_label_set_text(label_user, "User: ...");
    lv_obj_align(label_user, LV_ALIGN_LEFT_MID, 5, -40);

    // AI reply
    label_ai = lv_label_create(scr);
    lv_label_set_text(label_ai, "Jarvis: ...");
    lv_obj_align(label_ai, LV_ALIGN_LEFT_MID, 5, 0);

    // To-Do / Alarm list
    todo_list = lv_list_create(scr);
    lv_obj_set_size(todo_list, 220, 100);
    lv_obj_align(todo_list, LV_ALIGN_BOTTOM_MID, 0, -30);

    // Status/footer
    label_status = lv_label_create(scr);
    lv_label_set_text(label_status, "⏳ Waiting for wake word...");
    lv_obj_align(label_status, LV_ALIGN_BOTTOM_MID, 0, -5);
}

// === Update functions ===
void update_user_text(const char *text) {
    lv_label_set_text_fmt(label_user, "User: %s", text);
}

void update_ai_text(const char *text) {
    lv_label_set_text_fmt(label_ai, "Jarvis: %s", text);
}

void update_status(const char *text) {
    lv_label_set_text(label_status, text);
}

// === To-Do/Alarm functions ===
void add_todo_item(const char *task) {
    lv_obj_t *btn = lv_list_add_btn(todo_list, LV_SYMBOL_BELL, task);
    (void)btn; // not used but can be extended
}

void clear_todo_list() {
    lv_obj_clean(todo_list);
}
