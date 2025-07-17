#ifndef UI_H
#define UI_H

#include <lvgl.h>

void ui_init(void);
void ui_update_display(const char *turn, int distance, const char *time_str);

#endif // UI_H
