#include <cassert>
#include <cstdint>
#include <iostream>

namespace {

struct Metrics {
    uint32_t produced = 0;
    uint32_t consumed = 0;
    uint32_t overwritten = 0;
    uint32_t dropped_logs = 0;
    uint32_t peak_log_depth = 0;
    uint32_t worst_processing_delay_ms = 0;
};

Metrics run_condition(uint32_t quote_period_ms) {
    constexpr uint32_t duration_ms = 60000;
    constexpr uint32_t tick_ms = 10;
    constexpr uint32_t task_service_ms = 750;
    constexpr uint32_t log_service_ms = 1000;
    constexpr uint32_t log_capacity = 16;

    Metrics metrics;
    bool price_pending = false;
    uint32_t pending_published_at = 0;
    uint32_t next_task_available_at = 0;
    uint32_t log_depth = 0;

    for (uint32_t now = 0; now < duration_ms; now += tick_ms) {
        if (now % quote_period_ms == 0) {
            ++metrics.produced;
            if (price_pending) {
                ++metrics.overwritten;
            }
            price_pending = true;
            pending_published_at = now;
        }

        if (price_pending && now >= next_task_available_at) {
            ++metrics.consumed;
            const uint32_t delay = now - pending_published_at;
            if (delay > metrics.worst_processing_delay_ms) {
                metrics.worst_processing_delay_ms = delay;
            }
            price_pending = false;
            next_task_available_at = now + task_service_ms;

            if (log_depth == log_capacity) {
                ++metrics.dropped_logs;
            } else {
                ++log_depth;
                if (log_depth > metrics.peak_log_depth) {
                    metrics.peak_log_depth = log_depth;
                }
            }
        }

        if (now % log_service_ms == 0 && log_depth > 0) {
            --log_depth;
        }
    }

    return metrics;
}

void print_metrics(const char* label, const Metrics& metrics) {
    std::cout << label << ','
              << metrics.produced << ','
              << metrics.consumed << ','
              << metrics.overwritten << ','
              << metrics.dropped_logs << ','
              << metrics.peak_log_depth << ','
              << metrics.worst_processing_delay_ms << '\n';
}

}  // namespace

int main() {
    const Metrics low = run_condition(5000);
    const Metrics high = run_condition(500);
    assert(low.produced < high.produced);
    assert(high.overwritten > low.overwritten);
    assert(high.peak_log_depth > low.peak_log_depth);

    std::cout << "condicao,produzidos,consumidos,sobrescritos,logs_descartados,"
                 "pico_fila_log,pior_atraso_ms\n";
    print_metrics("baixa_5000ms", low);
    print_metrics("alta_500ms", high);
    return 0;
}
