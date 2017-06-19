#ifndef Buttons_h
#define Buttons_h

#define HOLD_TYPE_NONE -1
#define HOLD_TYPE_TAP 0
#define HOLD_TYPE_PRESS 1
#define HOLD_TYPE_MEDIUM 2
#define HOLD_TYPE_LONG 3
#define HOLD_TYPE_VERY_LONG 4

#define HOLD_DURATION_TAP 0 // milliseconds
#define HOLD_DURATION_PRESS 200 // milliseconds
#define HOLD_DURATION_MEDIUM 500 // milliseconds
#define HOLD_DURATION_LONG 2000 // milliseconds
#define HOLD_DURATION_VERY_LONG 3500 // milliseconds

namespace Buttons{ 
  int getHoldType(unsigned long duration);
}

#endif


