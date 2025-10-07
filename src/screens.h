#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *rit_screen;
    lv_obj_t *bill_bot_screen;
    lv_obj_t *status_screen;
    lv_obj_t *obj0;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_RIT_SCREEN = 1,
    SCREEN_ID_BILL_BOT_SCREEN = 2,
    SCREEN_ID_STATUS_SCREEN = 3,
};

void create_screen_rit_screen();
void tick_screen_rit_screen();

void create_screen_bill_bot_screen();
void tick_screen_bill_bot_screen();

void create_screen_status_screen();
void tick_screen_status_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/