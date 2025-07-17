#include "../re_logo/re_logo.h"
#include "../ui/ui.h"
#include <stdio.h>
#include <esp_netif.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include "arrow/arrow.h"

static lv_obj_t *label_direction;
static lv_obj_t *label_distance;
static lv_obj_t *label_time;
static lv_obj_t *label_ip;
static lv_obj_t *label_ble_msg = NULL;

static void update_ip_label_cb(lv_timer_t *timer)
{
    bsp_display_lock(0);
    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
    {
        char ipbuf[32];
        snprintf(ipbuf, sizeof(ipbuf), "IP: %d.%d.%d.%d", IP2STR(&ip_info.ip));
        lv_label_set_text(label_ip, ipbuf);
    }
    else
    {
        lv_label_set_text(label_ip, "IP: --");
    }
    bsp_display_unlock();
}

void ui_update_display(const char *turn, int distance, const char *time_str)
{
    bsp_display_lock(0);
    char buf[64];
    snprintf(buf, sizeof(buf), "Girar: %s", turn);
    lv_label_set_text(label_direction, buf);
    snprintf(buf, sizeof(buf), "Distancia: %d m", distance);
    lv_label_set_text(label_distance, buf);
    snprintf(buf, sizeof(buf), "Hora: %s", time_str);
    lv_label_set_text(label_time, buf);
    // Actualiza la IP
    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
    {
        char ipbuf[32];
        snprintf(ipbuf, sizeof(ipbuf), "IP: %d.%d.%d.%d", IP2STR(&ip_info.ip));
        lv_label_set_text(label_ip, ipbuf);
    }
    else
    {
        lv_label_set_text(label_ip, "IP: --");
    }
    bsp_display_unlock();
}

void principal_ui(void)
{
    bsp_display_lock(0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    label_direction = lv_label_create(lv_scr_act());
    lv_label_set_text(label_direction, "Girar: --");
    lv_obj_set_style_text_color(label_direction, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_direction, &lv_font_montserrat_22, 0);
    lv_obj_align(label_direction, LV_ALIGN_TOP_MID, 0, 30);
    label_distance = lv_label_create(lv_scr_act());
    lv_label_set_text(label_distance, "Distancia: -- m");
    lv_obj_set_style_text_color(label_distance, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_distance, &lv_font_montserrat_22, 0);
    lv_obj_align(label_distance, LV_ALIGN_TOP_MID, 0, 80);
    label_time = lv_label_create(lv_scr_act());
    lv_label_set_text(label_time, "Hora: --");
    lv_obj_set_style_text_color(label_time, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_16, 0);
    lv_obj_align(label_time, LV_ALIGN_TOP_MID, 0, 130);
    label_ip = lv_label_create(lv_scr_act());
    lv_label_set_text(label_ip, "IP: --");
    lv_obj_set_style_text_color(label_ip, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_ip, &lv_font_montserrat_16, 0);
    lv_obj_align(label_ip, LV_ALIGN_BOTTOM_MID, 0, -10);

    // Flecha central (derecha)
    lv_obj_t *arrow_c = arrow_right(lv_scr_act());
    lv_obj_align(arrow_c, LV_ALIGN_CENTER, 0, 0);
    // Flecha arriba
    lv_obj_t *arrow_u = arrow_up(lv_scr_act());
    lv_obj_align(arrow_u, LV_ALIGN_CENTER, 0, -80);
    // Flecha abajo
    lv_obj_t *arrow_d = arrow_down(lv_scr_act());
    lv_obj_align(arrow_d, LV_ALIGN_CENTER, 0, 80);
    // Flecha izquierda
    lv_obj_t *arrow_l = arrow_left(lv_scr_act());
    lv_obj_align(arrow_l, LV_ALIGN_CENTER, -80, 0);

    bsp_display_unlock();
    lv_timer_create(update_ip_label_cb, 1000, NULL);
}

void ui_init(void)
{
    splash_screen_royal_enfield();
    vTaskDelay(pdMS_TO_TICKS(4000));
    lv_obj_clean(lv_scr_act());
    principal_ui();
}

void ui_show_ble_message(const char *msg)
{
    if (!label_ble_msg)
    {
        label_ble_msg = lv_label_create(lv_scr_act());
        lv_obj_align(label_ble_msg, LV_ALIGN_TOP_MID, 0, 180);
        lv_obj_set_style_text_font(label_ble_msg, &lv_font_montserrat_22, 0);
    }
    lv_label_set_text(label_ble_msg, msg);
}