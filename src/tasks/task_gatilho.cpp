#include "tasks/task_gatilho.h"

#include "config.h"
#include "esp_timer.h"
#include "services/logger.h"
#include "system/queues.h"
#include "system/trigger.h"

namespace {

const char* crossing_name(PriceCrossing crossing) {
    return crossing == PriceCrossing::Upward ? "ascendente" : "descendente";
}

}  // namespace

void task_gatilho(void* argument) {
    auto* context = static_cast<AppContext*>(argument);
    if (context == nullptr) {
        vTaskDelete(nullptr);
        return;
    }

    TriggerStateMachine trigger(kTriggerThreshold, kRearmHysteresis);
    PriceMessage message{};
    uint32_t previous_sequence = 0;
    while (true) {
        if (!queues_receive_price(context, &message, portMAX_DELAY)) {
            continue;
        }

        if (previous_sequence != 0 && message.sequence > previous_sequence + 1) {
            const uint32_t missed = message.sequence - previous_sequence - 1;
            context->missed_prices.fetch_add(missed, std::memory_order_relaxed);
            logger_log(context, "T1 observou %lu cotacao(oes) sobrescrita(s)/perdida(s).",
                       static_cast<unsigned long>(missed));
        }
        previous_sequence = message.sequence;

        const int64_t detected_at_us = esp_timer_get_time();
        const TriggerUpdate update = trigger.update(message.price);
        if (!update.valid) {
            logger_log(context, "T1 rejeitou cotacao invalida (seq=%lu).",
                       static_cast<unsigned long>(message.sequence));
            continue;
        }
        if (update.first_price) {
            logger_log(context, "T1 recebeu primeira cotacao valida; sem cruzamento.");
        }
        if (update.rearmed) {
            logger_log(context, "T1 rearmado pela histerese herdada de %.2f.",
                       static_cast<double>(kRearmHysteresis));
        }
        if (update.triggered) {
            const int64_t queue_delay_us = detected_at_us - message.published_at_us;
            logger_log(context,
                       "T1 detectou cruzamento %s (seq=%lu, preco=%.2f, atraso_fila_us=%lld). "
                       "Acao de ordem nao configurada; nenhuma ordem enviada.",
                       crossing_name(update.crossing),
                       static_cast<unsigned long>(message.sequence),
                       static_cast<double>(message.price),
                       static_cast<long long>(queue_delay_us));
        }
    }
}
