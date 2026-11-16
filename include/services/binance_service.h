#pragma once

#include "esp_err.h"

esp_err_t binance_fetch_quote(float* price);
esp_err_t binance_send_order(const char* side, const char* symbol, float quantity);