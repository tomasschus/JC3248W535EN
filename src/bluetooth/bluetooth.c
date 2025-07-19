#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_gap.h"
#include "esp_log.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "../ui/ui.h"
#include <inttypes.h>
#include <string.h>

static const char *TAG = "BLE_NIMBLE";
static const char *BLE_TAG = "Drivy-5xg7";

#define BLE_SERVICE_UUID 0x181C
#define BLE_CHAR_UUID_RX 0x2A56

extern void ui_show_ble_json(const char *json);

// Declaraciones anticipadas
static int ble_gap_event_cb(struct ble_gap_event *event, void *arg);
static void start_advertising(void);
static void stop_advertising(void);

static char ble_rx_buffer[512];
static int ble_rx_buffer_len = 0;

// Callback de acceso a la característica RX
static int ble_rx_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                            struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
    {
        int len = ctxt->om->om_len;
        if (ble_rx_buffer_len + len < sizeof(ble_rx_buffer) - 1)
        {
            memcpy(ble_rx_buffer + ble_rx_buffer_len, ctxt->om->om_data, len);
            ble_rx_buffer_len += len;
            ble_rx_buffer[ble_rx_buffer_len] = '\0';
            ESP_LOGI(TAG, "Recibido por BLE: %s", ble_rx_buffer);

            if (strchr(ble_rx_buffer, '}'))
            {
                ui_show_ble_json(ble_rx_buffer);
                ble_rx_buffer_len = 0;
            }
        }
        else
        {
            ble_rx_buffer_len = 0;
        }
        return 0;
    }
    return 0;
}

static const struct ble_gatt_svc_def gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SERVICE_UUID),
        .characteristics = (struct ble_gatt_chr_def[]){
            {
                .uuid = BLE_UUID16_DECLARE(BLE_CHAR_UUID_RX),
                .access_cb = ble_rx_access_cb,
                .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            {0},
        },
    },
    {0},
};

static void start_advertising(void)
{
    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));

    fields.name = (uint8_t *)BLE_TAG;
    fields.name_len = strlen(BLE_TAG);
    fields.name_is_complete = 1;

    ble_gap_adv_set_fields(&fields);

    struct ble_gap_adv_params adv_params = {0};
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    ble_gap_adv_start(0, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event_cb, NULL);
    ESP_LOGI(TAG, "BLE advertising iniciado");
}

static void stop_advertising(void)
{
    ble_gap_adv_stop();
    ESP_LOGI(TAG, "BLE advertising detenido");
}

void ble_app_on_sync(void);

static int ble_gap_event_cb(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0)
        {
            ESP_LOGI(TAG, "Dispositivo conectado, deteniendo advertising");
            activate_bluetooth_icon();
            stop_advertising();
        }
        else
        {
            ESP_LOGI(TAG, "Fallo al conectar, reiniciando advertising");
            start_advertising();
        }
        break;
    case BLE_GAP_EVENT_DISCONNECT:
        ble_rx_buffer_len = 0;
        start_advertising();
        desactivate_bluetooth_icon();
        break;
    default:
        break;
    }
    return 0;
}

static void ble_host_task(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void ble_app_on_sync(void)
{
    ESP_LOGI(TAG, "BLE stack sincronizado");

    ble_svc_gap_init();
    ble_svc_gatt_init();
    ble_svc_gap_device_name_set(BLE_TAG);

    ble_gatts_count_cfg(gatt_svcs);
    ble_gatts_add_svcs(gatt_svcs);
    ble_gatts_start();

    start_advertising();
}

void bluetooth_init(void)
{
    esp_nimble_hci_init();
    nimble_port_init();
    ble_hs_cfg.sync_cb = ble_app_on_sync;
    nimble_port_freertos_init(ble_host_task);
}
