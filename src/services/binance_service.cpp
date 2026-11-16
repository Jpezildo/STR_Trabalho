#include "services/binance_service.h"

#include <cmath>
#include <cstdlib>
#include <cstring>

#include "config.h"
#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"

namespace {

struct HttpResponse {
    char body[256]{};
    size_t length = 0;
    bool overflow = false;
};

esp_err_t response_event_handler(esp_http_client_event_t* event) {
    if (event == nullptr || event->user_data == nullptr ||
        event->event_id != HTTP_EVENT_ON_DATA || event->data == nullptr ||
        event->data_len <= 0) {
        return ESP_OK;
    }

    auto* response = static_cast<HttpResponse*>(event->user_data);
    const size_t incoming = static_cast<size_t>(event->data_len);
    const size_t capacity = sizeof(response->body) - response->length - 1;
    if (incoming > capacity) {
        response->overflow = true;
        return ESP_FAIL;
    }

    std::memcpy(response->body + response->length, event->data, incoming);
    response->length += incoming;
    response->body[response->length] = '\0';
    return ESP_OK;
}

}  // namespace

esp_err_t binance_fetch_quote(float* price) {
    if (price == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    HttpResponse response;
    esp_http_client_config_t config{};
    config.url = kQuoteUrl;
    config.event_handler = response_event_handler;
    config.user_data = &response;
    config.timeout_ms = kHttpTimeoutMs;
    config.crt_bundle_attach = esp_crt_bundle_attach;

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    const esp_err_t request_result = esp_http_client_perform(client);
    const int status_code = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if (request_result != ESP_OK) {
        return request_result;
    }
    if (response.overflow) {
        return ESP_ERR_INVALID_SIZE;
    }
    if (status_code != 200) {
        return ESP_FAIL;
    }

    cJSON* root = cJSON_Parse(response.body);
    if (root == nullptr) {
        return ESP_ERR_INVALID_RESPONSE;
    }

    const cJSON* price_item = cJSON_GetObjectItemCaseSensitive(root, "price");
    char* end = nullptr;
    float parsed_price = 0.0F;
    bool valid = false;

    if (cJSON_IsString(price_item) && price_item->valuestring != nullptr) {
        parsed_price = strtof(price_item->valuestring, &end);
        valid = end != price_item->valuestring && *end == '\0' &&
               std::isfinite(parsed_price) && parsed_price > 0.0F;
    } else if (cJSON_IsNumber(price_item)) {
        parsed_price = static_cast<float>(price_item->valuedouble);
        valid = std::isfinite(parsed_price) && parsed_price > 0.0F;
    }

    cJSON_Delete(root);
    if (!valid) {
        return ESP_ERR_INVALID_RESPONSE;
    }

    *price = parsed_price;
    return ESP_OK;
}

esp_err_t binance_send_order(const char* side, const char* symbol, float quantity) {
    if (side == nullptr || symbol == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }
    if (quantity <= 0.0F) {
        return ESP_ERR_INVALID_ARG;
    }

    if (STR_BINANCE_API_KEY[0] == '\0' || STR_BINANCE_API_SECRET[0] == '\0') {
        return ESP_ERR_NOT_SUPPORTED;
    }

    (void)side;
    (void)symbol;
    return ESP_ERR_NOT_SUPPORTED;
}
