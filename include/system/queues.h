#pragma once

#include "system/app_context.h"

bool queues_init(AppContext* context);
bool queues_publish_price(AppContext* context, const PriceMessage* message);
bool queues_receive_price(AppContext* context, PriceMessage* message, TickType_t wait);
bool queues_send_log(AppContext* context, const LogMessage* message);
bool queues_receive_log(AppContext* context, LogMessage* message, TickType_t wait);