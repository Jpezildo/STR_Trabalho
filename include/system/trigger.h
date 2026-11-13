#pragma once

enum class PriceCrossing {
    None,
    Upward,
    Downward,
};

struct TriggerUpdate {
    bool valid = false;
    bool first_price = false;
    bool triggered = false;
    bool rearmed = false;
    PriceCrossing crossing = PriceCrossing::None;
};

class TriggerStateMachine {
public:
    TriggerStateMachine(float threshold, float rearm_hysteresis);
    TriggerUpdate update(float price);
    bool armed() const;

private:
    float threshold_;
    float rearm_hysteresis_;
    float previous_price_ = 0.0F;
    bool has_previous_price_ = false;
    bool armed_ = true;
};
