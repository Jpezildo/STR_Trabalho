#include "system/synchronization.h"

bool synchronization_init(AppContext* context) {
    if (context == nullptr) {
        return false;
    }
    context->system_events = xEventGroupCreate();
    return context->system_events != nullptr;
}

void synchronization_set_wifi(AppContext* context, bool connected) {
    if (context == nullptr || context->system_events == nullptr) {
        return;
    }
    if (connected) {
        xEventGroupSetBits(context->system_events, kWifiConnectedBit);
    } else {
        xEventGroupClearBits(context->system_events, kWifiConnectedBit);
    }
}

bool synchronization_wifi_connected(const AppContext* context) {
    return context != nullptr && context->system_events != nullptr &&
           (xEventGroupGetBits(context->system_events) & kWifiConnectedBit) != 0;
}
