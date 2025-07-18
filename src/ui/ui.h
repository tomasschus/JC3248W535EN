#ifndef UI_H
#define UI_H

#include <lvgl.h>

void ui_init(void);
void ui_show_ble_json(const char *json);
void activate_bluetooth_icon(void);
void desactivate_bluetooth_icon(void);

#endif // UI_H
