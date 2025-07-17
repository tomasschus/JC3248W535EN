
#include <nvs_flash.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include "ui/ui.h"
#include "wifi/wifi_manager.h"
#include "webserver/webserver.h"
#include "bluetooth/bluetooth.h"

void app_main()
{
  ESP_ERROR_CHECK(nvs_flash_init());

  bsp_display_cfg_t cfg = {
      .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
      .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
      .rotate = LV_DISP_ROT_270,
  };
  bsp_display_start_with_config(&cfg);
  bsp_display_backlight_on();

  ui_init();
  wifi_manager_init();
  webserver_start();
  bluetooth_init();
}
