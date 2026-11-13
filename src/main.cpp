#include <cstdlib>
#include <cstring>

#include "config.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "services/logger.h"
#include "system/app_context.h"
#include "system/queues.h"
#include "system/synchronization.h"
#include "tasks/task_consulta.h"
#include "tasks/task_gatilho.h"
#include "tasks/task_registro.h"

namespace {

constexpr char kTag[] = "STR_APP";
AppContext app_context;
esp_event_handler_instance_t wifi_event_instance;
esp_event_handler_instance_t ip_event_instance;

void wifi_event_handler(void* argument, esp_event_base_t event_base,
                        int32_t event_id, void*) {
    auto* context = static_cast<AppContext*>(argument);
    if (context == nullptr || event_base != WIFI_EVENT) {
        return;
    }

    if (event_id == WIFI_EVENT_STA_START) {
        const esp_err_t result = esp_wifi_connect();
        if (result != ESP_OK) {
            logger_log(context, "Falha ao iniciar conexao Wi-Fi: %s.",
                       esp_err_to_name(result));
        }
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        synchronization_set_wifi(context, false);
        const esp_err_t result = esp_wifi_connect();
        if (result != ESP_OK) {
            logger_log(context, "Falha ao solicitar reconexao Wi-Fi: %s.",
                       esp_err_to_name(result));
        }
    }
}

void ip_event_handler(void* argument, esp_event_base_t event_base,
                      int32_t event_id, void*) {
    auto* context = static_cast<AppContext*>(argument);
    if (context != nullptr && event_base == IP_EVENT &&
        event_id == IP_EVENT_STA_GOT_IP) {
        synchronization_set_wifi(context, true);
        logger_log(context, "Wi-Fi conectado; consultas habilitadas.");
    }
}

esp_err_t start_wifi() {
    ESP_RETURN_ON_ERROR(esp_netif_init(), kTag, "Falha ao inicializar netif");
    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), kTag,
                        "Falha ao criar event loop");
    if (esp_netif_create_default_wifi_sta() == nullptr) {
        return ESP_FAIL;
    }

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(esp_wifi_init(&wifi_init_config), kTag,
                        "Falha ao inicializar Wi-Fi");
    ESP_RETURN_ON_ERROR(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                            wifi_event_handler, &app_context,
                                            &wifi_event_instance),
        kTag, "Falha ao registrar eventos Wi-Fi");
    ESP_RETURN_ON_ERROR(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                            ip_event_handler, &app_context,
                                            &ip_event_instance),
        kTag, "Falha ao registrar evento IP");

    wifi_config_t wifi_config{};
    const size_t ssid_length = std::strlen(STR_WIFI_SSID);
    const size_t password_length = std::strlen(STR_WIFI_PASSWORD);
    if (ssid_length > sizeof(wifi_config.sta.ssid) ||
        password_length > sizeof(wifi_config.sta.password)) {
        return ESP_ERR_INVALID_SIZE;
    }
    std::memcpy(wifi_config.sta.ssid, STR_WIFI_SSID, ssid_length);
    std::memcpy(wifi_config.sta.password, STR_WIFI_PASSWORD,
                password_length);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), kTag,
                        "Falha ao configurar modo Wi-Fi");
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_config), kTag,
                        "Falha ao configurar rede Wi-Fi");
    return esp_wifi_start();
}

void start_task(const char* name, TaskFunction_t function,
                uint32_t stack_size, UBaseType_t priority) {
    if (xTaskCreate(function, name, stack_size, &app_context, priority, nullptr) !=
        pdPASS) {
        ESP_LOGE(kTag, "Falha ao criar tarefa %s", name);
        abort();
    }
}

}  // namespace

extern "C" void app_main() {
    esp_err_t nvs_result = nvs_flash_init();
    if (nvs_result == ESP_ERR_NVS_NO_FREE_PAGES ||
        nvs_result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvs_result = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvs_result);

    if (!queues_init(&app_context) ||
        !synchronization_init(&app_context)) {
        ESP_LOGE(kTag, "Falha ao criar recursos FreeRTOS");
        abort();
    }
    start_task("T3_Registro", task_registro, 3072, 1);

    app_context.simulation_mode =
        STR_WIFI_SSID[0] == '\0' || STR_WIFI_PASSWORD[0] == '\0';
    if (app_context.simulation_mode) {
        logger_log(&app_context,
                   "Sem credenciais locais: modo de simulacao ativado; nenhuma ordem sera enviada.");
    } else {
        const esp_err_t wifi_result = start_wifi();
        if (wifi_result != ESP_OK) {
            ESP_LOGE(kTag, "Falha ao iniciar Wi-Fi: %s",
                     esp_err_to_name(wifi_result));
            abort();
        }
    }

    start_task("T2_Consulta", task_consulta, 6144, 2);
    start_task("T1_Gatilho", task_gatilho, 4096, 3);
}
