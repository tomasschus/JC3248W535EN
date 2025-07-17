#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "esp_log.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "../ui/ui.h"

static const char *TAG = "BLE_NIMBLE";

// UUIDs personalizados
#define BLE_SERVICE_UUID 0x181C
#define BLE_CHAR_UUID_RX 0x2A56

// Prototipo de función para actualizar la UI
extern void ui_show_ble_message(const char *msg);

// Callback de acceso a la característica RX
static int ble_rx_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                            struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
    {
        // Copia el dato recibido y lo muestra en la UI
        char buf[64] = {0};
        int len = ctxt->om->om_len > 63 ? 63 : ctxt->om->om_len;
        memcpy(buf, ctxt->om->om_data, len);
        buf[len] = '\0';
        ESP_LOGI(TAG, "Recibido por BLE: %s", buf);
        ui_show_ble_message(buf); // Actualiza la pantalla
        return 0;
    }
    return BLE_ATT_ERR_UNLIKELY;
}

// Definición del servicio y característica
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
            {0}, // Fin de características
        },
    },
    {0}, // Fin de servicios
};

void ble_app_on_sync(void); // <-- PROTOTIPO AQUÍ

// Callback de eventos GAP
static int ble_gap_event_cb(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0)
        {
            ESP_LOGI(TAG, "Dispositivo conectado, deteniendo advertising");
            // No reiniciamos advertising aquí, solo permitimos una conexión
        }
        else
        {
            ESP_LOGI(TAG, "Fallo al conectar, reiniciando advertising");
            ble_app_on_sync(); // Reinicia advertising si la conexión falló
        }
        break;
    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "Dispositivo desconectado, reiniciando advertising");
        ble_app_on_sync(); // Reinicia advertising al desconectar
        break;
    default:
        break;
    }
    return 0;
}

// Tarea del host BLE
static void ble_host_task(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void ble_app_on_sync(void)
{
    ESP_LOGI(TAG, "BLE stack sincronizado");

    // Inicializa servicios estándar GAP y GATT
    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_svc_gap_device_name_set("MiESP32-BLE");

    // Registra el servicio GATT personalizado
    ble_gatts_count_cfg(gatt_svcs);
    ble_gatts_add_svcs(gatt_svcs);

    ble_gatts_start();

    // Advertising con nombre
    struct ble_hs_adv_fields fields = {0};
    fields.name = (uint8_t *)"MiESP32-BLE";
    fields.name_len = strlen("MiESP32-BLE");
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields);

    struct ble_gap_adv_params adv_params = {0};
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    // Usa el callback de eventos GAP para controlar conexiones
    ble_gap_adv_start(0, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event_cb, NULL);
    ESP_LOGI(TAG, "BLE advertising iniciado");
}

void bluetooth_init(void)
{
    esp_nimble_hci_init();
    nimble_port_init();
    ble_hs_cfg.sync_cb = ble_app_on_sync;
    // Pasa la tarea del host BLE aquí:
    nimble_port_freertos_init(ble_host_task);
}