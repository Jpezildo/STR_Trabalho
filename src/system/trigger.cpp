#include "system/trigger.h"

#include <cmath>

TriggerStateMachine::TriggerStateMachine(float threshold, float rearm_hysteresis)
    : threshold_(threshold),
      rearm_hysteresis_(rearm_hysteresis < 0.0F ? 0.0F : rearm_hysteresis) {}

TriggerUpdate TriggerStateMachine::update(float price) {
    TriggerUpdate result;
    if (!std::isfinite(price) || price <= 0.0F) {
        return result;
    }
    result.valid = true;

    if (!has_previous_price_) {
        previous_price_ = price;
        has_previous_price_ = true;
        result.first_price = true;
        return result;
    }

    if (!armed_) {
        previous_price_ = price;
        if (price <= threshold_ - rearm_hysteresis_) {
            armed_ = true;
            result.rearmed = true;
        }
        return result;
    }

    if (previous_price_ < threshold_ && price >= threshold_) {
        result.crossing = PriceCrossing::Upward;
    } else if (previous_price_ > threshold_ && price <= threshold_) {
        result.crossing = PriceCrossing::Downward;
    }

    previous_price_ = price;
    if (result.crossing != PriceCrossing::None) {
        armed_ = false;
        result.triggered = true;
    }
    return result;
}

bool TriggerStateMachine::armed() const {
    return armed_;
}
