#include "../ui/ui.h"
#include <stdio.h>
#include <esp_netif.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include "arrow/arrow.h"
#include "../re_logo/re_logo.h"
#include <cJSON.h>
#include "esp_log.h"

static lv_obj_t *icon_bt = NULL;
static lv_obj_t *label_hour = NULL;
static lv_obj_t *arrow_turn = NULL;
static lv_obj_t *label_turn = NULL;
static lv_obj_t *label_distance = NULL;
static lv_obj_t *label_time = NULL;
static lv_obj_t *label_speed = NULL;

void principal_ui(void)
{
    bsp_display_lock(0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);

    label_hour = lv_label_create(lv_scr_act());
    lv_label_set_text(label_hour, "HH:MM");
    lv_obj_set_style_text_color(label_hour, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_hour, &lv_font_montserrat_18, 0);
    lv_obj_align(label_hour, LV_ALIGN_TOP_RIGHT, -5, 5);

    icon_bt = lv_label_create(lv_scr_act());
    lv_label_set_text(icon_bt, LV_SYMBOL_BLUETOOTH);
    lv_obj_set_style_text_color(icon_bt, lv_color_hex(0x808080), 0);
    lv_obj_set_style_text_font(icon_bt, &lv_font_montserrat_22, 0);
    lv_obj_align(icon_bt, LV_ALIGN_TOP_LEFT, 10, 10);

    // // Flecha central
    // lv_obj_t *arrow_u = arrow_up(lv_scr_act());
    // lv_obj_align(arrow_u, LV_ALIGN_CENTER, 0, -40);

    // Label para giro (turn)
    label_turn = lv_label_create(lv_scr_act());
    lv_label_set_text(label_turn, "");
    lv_obj_set_style_text_color(label_turn, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_turn, &lv_font_montserrat_16, 0);
    lv_obj_align(label_turn, LV_ALIGN_CENTER, 0, 50);

    // Label para distancia
    label_distance = lv_label_create(lv_scr_act());
    lv_label_set_text(label_distance, "Dist: -- m");
    lv_obj_set_style_text_color(label_distance, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_distance, &lv_font_montserrat_18, 0);
    lv_obj_align(label_distance, LV_ALIGN_BOTTOM_LEFT, 10, -10);

    // Label para tiempo
    label_time = lv_label_create(lv_scr_act());
    lv_label_set_text(label_time, "Llegada a las --:--");
    lv_obj_set_style_text_color(label_time, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_18, 0);
    lv_obj_align(label_time, LV_ALIGN_BOTTOM_LEFT, 10, -40);

    // Label para velocidad
    label_speed = lv_label_create(lv_scr_act());
    lv_label_set_text(label_speed, "Vel: -- km/h");
    lv_obj_set_style_text_color(label_speed, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_speed, &lv_font_montserrat_18, 0);
    lv_obj_align(label_speed, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

    bsp_display_unlock();
}

void show_arrow(const char *direction)
{
    if (arrow_turn)
    {
        lv_obj_del(arrow_turn);
        arrow_turn = NULL;
    }

    if (direction == NULL)
        return;

    if (strcmp(direction, "up") == 0)
    {
        arrow_turn = arrow_up(lv_scr_act());
    }
    else if (strcmp(direction, "down") == 0)
    {
        arrow_turn = arrow_down(lv_scr_act());
    }
    else if (strcmp(direction, "left") == 0)
    {
        arrow_turn = arrow_left(lv_scr_act());
    }
    else if (strcmp(direction, "right") == 0)
    {
        arrow_turn = arrow_right(lv_scr_act());
    }

    if (arrow_turn)
        lv_obj_align(arrow_turn, LV_ALIGN_CENTER, 0, -40);
}

void activate_bluetooth_icon(void)
{
    if (icon_bt)
        lv_obj_set_style_text_color(icon_bt, lv_color_hex(0x00BFFF), 0);
}

void desactivate_bluetooth_icon(void)
{
    if (icon_bt)
        lv_obj_set_style_text_color(icon_bt, lv_color_hex(0x808080), 0);
}

void ui_init(void)
{
    splash_screen_royal_enfield();
    vTaskDelay(pdMS_TO_TICKS(2000));
    lv_obj_clean(lv_scr_act());
    principal_ui();
}

static void clean_json(char *json)
{
    char *src = json, *dst = json;
    while (*src)
    {
        if (src[0] == '\\' && src[1] == '"')
            src++;
        *dst++ = *src++;
    }
    *dst = '\0';
}

void ui_show_ble_json(const char *json)
{
    char clean_buf[512];
    strncpy(clean_buf, json, sizeof(clean_buf) - 1);
    clean_buf[sizeof(clean_buf) - 1] = '\0';
    clean_json(clean_buf);

    ESP_LOGI("UI", "JSON limpio: %s", clean_buf);
    cJSON *root = cJSON_Parse(clean_buf);
    if (!root)
    {
        ESP_LOGE("UI", "Error al parsear JSON");
        return;
    }

    bsp_display_lock(0);

    const cJSON *arrow = cJSON_GetObjectItemCaseSensitive(root, "arrow");
    ESP_LOGI("ARROW", "Recibido por BLE: %s", arrow ? arrow->valuestring : "NULL");
    show_arrow(arrow ? arrow->valuestring : NULL);

    const cJSON *status = cJSON_GetObjectItemCaseSensitive(root, "arrow_turn");
    if (status && cJSON_IsString(status) && status->valuestring)
    {
        lv_label_set_text(label_turn, status->valuestring);
    }

    const cJSON *hour_val = cJSON_GetObjectItemCaseSensitive(root, "hour");
    if (hour_val && cJSON_IsString(hour_val) && hour_val->valuestring)
    {
        lv_label_set_text(label_hour, hour_val->valuestring);
    }

    const cJSON *speed_val = cJSON_GetObjectItemCaseSensitive(root, "speed");
    if (speed_val && cJSON_IsString(speed_val) && speed_val->valuestring)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "Vel: %s", speed_val->valuestring);
        lv_label_set_text(label_speed, buf);
    }

    const cJSON *turn = cJSON_GetObjectItemCaseSensitive(root, "turn");
    if (turn && cJSON_IsString(turn) && turn->valuestring)
        lv_label_set_text(label_turn, turn->valuestring);

    const cJSON *distance_val = cJSON_GetObjectItemCaseSensitive(root, "distance");
    if (distance_val && cJSON_IsString(distance_val) && distance_val->valuestring)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "Dist: %s", distance_val->valuestring);
        lv_label_set_text(label_distance, buf);
    }

    const cJSON *time_val = cJSON_GetObjectItemCaseSensitive(root, "time");
    if (time_val && cJSON_IsString(time_val) && time_val->valuestring)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "Llegada a las %s", time_val->valuestring);
        lv_label_set_text(label_time, buf);
    }

    bsp_display_unlock();
    cJSON_Delete(root);
}