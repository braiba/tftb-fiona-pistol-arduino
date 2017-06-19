#include "Buttons.h"

int Buttons::getHoldType(unsigned long duration) {
  if (duration >= HOLD_DURATION_DEBUG) {
    return HOLD_TYPE_DEBUG;
  }
  if (duration >= HOLD_DURATION_LONG) {
    return HOLD_TYPE_LONG;
  }
  if (duration >= HOLD_DURATION_MEDIUM) {
    return HOLD_TYPE_MEDIUM;
  }
  if (duration >= HOLD_DURATION_PRESS) {
    return HOLD_TYPE_PRESS;
  }
  return HOLD_TYPE_TAP;
}
