#include <cassert>
#include <limits>

#include "system/trigger.h"

int main() {
    TriggerStateMachine upward(100.0F, 10.0F);
    assert(upward.update(90.0F).first_price);
    assert(!upward.update(95.0F).triggered);
    const TriggerUpdate up = upward.update(100.0F);
    assert(up.triggered && up.crossing == PriceCrossing::Upward);
    assert(!upward.armed());
    assert(!upward.update(110.0F).triggered);
    assert(upward.update(90.0F).rearmed);
    const TriggerUpdate up_again = upward.update(100.0F);
    assert(up_again.triggered && up_again.crossing == PriceCrossing::Upward);

    TriggerStateMachine downward(100.0F, 10.0F);
    assert(downward.update(110.0F).first_price);
    const TriggerUpdate down = downward.update(100.0F);
    assert(down.triggered && down.crossing == PriceCrossing::Downward);

    TriggerStateMachine invalid(100.0F, 10.0F);
    assert(invalid.update(110.0F).first_price);
    assert(!invalid.update(std::numeric_limits<float>::quiet_NaN()).valid);
    assert(!invalid.update(std::numeric_limits<float>::infinity()).valid);
    assert(!invalid.update(0.0F).valid);
    assert(invalid.update(90.0F).triggered);

    TriggerStateMachine no_false_crossing(100.0F, 10.0F);
    assert(no_false_crossing.update(120.0F).first_price);
    assert(!no_false_crossing.update(130.0F).triggered);
    assert(!no_false_crossing.update(110.0F).triggered);
    return 0;
}
