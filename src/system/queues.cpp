#include "system/queues.h"

bool queues_init(AppContext* context) {
    if (context == nullptr) {
        return false;
    }

    context->price_queue = xQueueCreate(1, sizeof(PriceMessage));
    context->log_queue = xQueueCreate(16, sizeof(LogMessage));
    if (context->price_queue == nullptr || context->log_queue == nullptr) {
        if (context->price_queue != nullptr) {
            vQueueDelete(context->price_queue);
            context->price_queue = nullptr;
        }
        if (context->log_queue != nullptr) {
            vQueueDelete(context->log_queue);
            context->log_queue = nullptr;
        }
        return false;
    }
    return true;
}

bool queues_publish_price(AppContext* context, const PriceMessage* message) {
    return context != nullptr && context->price_queue != nullptr &&
           message != nullptr &&
           xQueueOverwrite(context->price_queue, message) == pdPASS;
}

bool queues_receive_price(AppContext* context, PriceMessage* message, TickType_t wait) {
    return context != nullptr && context->price_queue != nullptr &&
           message != nullptr &&
           xQueueReceive(context->price_queue, message, wait) == pdPASS;
}

bool queues_send_log(AppContext* context, const LogMessage* message) {
    return context != nullptr && context->log_queue != nullptr &&
           message != nullptr &&
           xQueueSend(context->log_queue, message, 0) == pdPASS;
}

bool queues_receive_log(AppContext* context, LogMessage* message, TickType_t wait) {
    return context != nullptr && context->log_queue != nullptr &&
           message != nullptr &&
           xQueueReceive(context->log_queue, message, wait) == pdPASS;
}
