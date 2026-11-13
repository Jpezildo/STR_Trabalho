#pragma once

#if __has_include("secrets.h")
#include "secrets.h"
#else
#define STR_WIFI_SSID ""
#define STR_WIFI_PASSWORD ""
#define STR_BINANCE_API_KEY ""
#define STR_BINANCE_API_SECRET ""
#endif

inline constexpr char kQuoteUrl[] =
    "https://testnet.binance.vision/api/v3/ticker/price?symbol=BTCUSDT";
inline constexpr float kTriggerThreshold = 61000.0F;
inline constexpr float kRearmHysteresis = 100.0F;
inline constexpr unsigned kQuotePeriodMs = 5000;
inline constexpr unsigned kHttpTimeoutMs = 5000;
