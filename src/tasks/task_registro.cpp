#include "tasks/task_registro.h"

#include "esp_log.h"
#include "system/app_context.h"
#include "system/queues.h"

namespace {
constexpr char kTag[] = "T3_REGISTRO";
}

void task_registro(void* argument) {
    auto* context = static_cast<AppContext*>(argument);
    LogMessage message{};
    if (context == nullptr) {
        vTaskDelete(nullptr);
        return;
    }
    TickType_t last_report = xTaskGetTickCount();
    uint32_t reported_drops = 0;
    uint32_t reported_missed_prices = 0;

    while (true) {
        if (queues_receive_log(context, &message, pdMS_TO_TICKS(1000))) {
            ESP_LOGI(kTag, "[%lu] %s",
                     static_cast<unsigned long>(message.sequence), message.text);
        }
        const TickType_t now = xTaskGetTickCount();
        if (now - last_report >= pdMS_TO_TICKS(10000)) {
            const uint32_t dropped =
                context->dropped_logs.load(std::memory_order_relaxed);
            const uint32_t missed_prices =
                context->missed_prices.load(std::memory_order_relaxed);
            if (dropped != reported_drops ||
                missed_prices != reported_missed_prices) {
                ESP_LOGW(kTag,
                         "Logs descartados=%lu; cotacoes perdidas=%lu; pico da fila=%lu/16.",
                         static_cast<unsigned long>(dropped),
                         static_cast<unsigned long>(missed_prices),
                         static_cast<unsigned long>(
                             context->peak_log_queue_depth.load(std::memory_order_relaxed)));
                reported_drops = dropped;
                reported_missed_prices = missed_prices;
            }
            last_report = now;
        }
    }
}
