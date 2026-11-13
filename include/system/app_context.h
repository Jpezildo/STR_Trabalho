#pragma once

#include <atomic>
#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

struct PriceMessage {
    float price;
    uint32_t sequence;
    int64_t published_at_us;
};

struct LogMessage {
    uint32_t sequence;
    char text[160];
};

struct AppContext {
    QueueHandle_t price_queue = nullptr;
    QueueHandle_t log_queue = nullptr;
    EventGroupHandle_t system_events = nullptr;
    std::atomic<uint32_t> log_sequence{0};
    std::atomic<uint32_t> dropped_logs{0};
    std::atomic<uint32_t> missed_prices{0};
    std::atomic<uint32_t> peak_log_queue_depth{0};
    bool simulation_mode = false;
};
