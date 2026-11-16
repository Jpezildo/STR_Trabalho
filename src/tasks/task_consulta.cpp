#include "tasks/task_consulta.h"

#include <array>

#include "config.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "services/binance_service.h"
#include "services/logger.h"
#include "system/queues.h"
#include "system/synchronization.h"

void task_consulta(void* argument) {
    auto* context = static_cast<AppContext*>(argument);
    if (context == nullptr) {
        vTaskDelete(nullptr);
        return;
    }

    constexpr std::array<float, 4> simulated_offsets{-200.0F, -50.0F, 50.0F, -150.0F};
    uint32_t sequence = 0;
    TickType_t last_wake = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(kQuotePeriodMs));
        float price = 0.0F;

        if (context->simulation_mode) {
            const float offset = simulated_offsets[sequence % simulated_offsets.size()];
            price = kTriggerThreshold + offset;
        } else {
            if (!synchronization_wifi_connected(context)) {
                logger_log(context, "T2 aguardando conexao Wi-Fi.");
                continue;
            }
            const esp_err_t result = binance_fetch_quote(&price);
            if (result != ESP_OK) {
                logger_log(context, "T2 falha na consulta de cotacao (erro=%s).",
                           esp_err_to_name(result));
                continue;
            }
        }

        PriceMessage message{};
        message.price = price;
        message.sequence = ++sequence;
        message.published_at_us = esp_timer_get_time();
        if (!queues_publish_price(context, &message)) {
            logger_log(context, "T2 nao conseguiu publicar a cotacao (seq=%lu).",
                       static_cast<unsigned long>(message.sequence));
            continue;
        }
        logger_log(context, "T2 publicou cotacao %.2f (seq=%lu, modo=%s).",
                   static_cast<double>(price),
                   static_cast<unsigned long>(message.sequence),
                   context->simulation_mode ? "simulacao" : "testnet");
    }
}
