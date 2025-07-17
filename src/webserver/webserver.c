
#include "webserver.h"
#include <esp_http_server.h>
#include <esp_log.h>
#include <cJSON.h>
#include "../ui/ui.h"

static const char *TAG = "webserver";

static esp_err_t navigate_post_handler(httpd_req_t *req)
{
    char buf[256];
    int ret, remaining = req->content_len;
    if (remaining >= sizeof(buf))
    {
        ESP_LOGW(TAG, "Payload muy grande");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    int received = 0;
    while (remaining > 0)
    {
        ret = httpd_req_recv(req, buf + received, remaining);
        if (ret <= 0)
        {
            ESP_LOGE(TAG, "Error leyendo POST");
            return ESP_FAIL;
        }
        received += ret;
        remaining -= ret;
    }
    buf[received] = '\0';

    ESP_LOGI(TAG, "POST recibido: %s", buf);

    cJSON *json = cJSON_Parse(buf);
    if (!json)
    {
        ESP_LOGE(TAG, "Error parseando JSON");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *turn = cJSON_GetObjectItemCaseSensitive(json, "turn");
    cJSON *distance = cJSON_GetObjectItemCaseSensitive(json, "distance");
    cJSON *time = cJSON_GetObjectItemCaseSensitive(json, "time");

    if (!cJSON_IsString(turn) || !cJSON_IsNumber(distance) || !cJSON_IsString(time))
    {
        ESP_LOGE(TAG, "JSON con formato incorrecto");
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad Request");
        return ESP_FAIL;
    }

    ui_update_display(turn->valuestring, distance->valueint, time->valuestring);
    cJSON_Delete(json);
    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}

esp_err_t webserver_start(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t navigate_uri = {
            .uri = "/navigate",
            .method = HTTP_POST,
            .handler = navigate_post_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &navigate_uri);
        ESP_LOGI(TAG, "Servidor HTTP iniciado");
        return ESP_OK;
    }
    return ESP_FAIL;
}
