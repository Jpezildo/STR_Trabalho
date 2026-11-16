#include "services/logger.h"

#include <cstdarg>
#include <cstdio>

#include "system/queues.h"

bool logger_log(AppContext* context, const char* format, ...) {
    if (context == nullptr || format == nullptr) {
        return false;
    }

    LogMessage message{};
    message.sequence = context->log_sequence.fetch_add(1, std::memory_order_relaxed) + 1;
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(message.text, sizeof(message.text), format, arguments);
    va_end(arguments);

    if (!queues_send_log(context, &message)) {
        context->dropped_logs.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    const uint32_t depth = static_cast<uint32_t>(
        uxQueueMessagesWaiting(context->log_queue));
    uint32_t peak = context->peak_log_queue_depth.load(std::memory_order_relaxed);
    while (depth > peak &&
           !context->peak_log_queue_depth.compare_exchange_weak(
               peak, depth, std::memory_order_relaxed)) {
    }
    return true;
}
