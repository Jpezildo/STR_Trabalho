#pragma once

#include "system/app_context.h"

constexpr EventBits_t kWifiConnectedBit = 1U << 0;

bool synchronization_init(AppContext* context);
void synchronization_set_wifi(AppContext* context, bool connected);
bool synchronization_wifi_connected(const AppContext* context);