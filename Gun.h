/*
  Gun.h - Defines the modes of the gun
*/
#ifndef Gun_h
#define Gun_h

#include "Barrel.h"
#include "GunModes.h"

class GunMode;

class Gun {
  public:
    Gun();
    
    Barrel* _barrels[3];

    void tick();
    boolean onTriggerPressed();
    boolean onTriggerHeld(int holdType);
    boolean onTriggerReleased(int holdType);
    
    void setGunMode(GunMode* newgunMode);
    void resetColourMasks();
    void clearColourMasks();
    
  private:
    CRGB _leds[NUM_LEDS];
    GunMode* _gunMode;
    
    bool _triggerHeld = false;
    bool _triggerResolved = false;
    unsigned long _triggerHoldStart = 0;
    int _triggerHoldType = 0;

    void checkTrigger();
};


#endif
