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
    boolean triggerPressed();
    boolean triggerHeld(int holdType);
    boolean triggerReleased(int holdType);
    
    void setGunMode(GunMode* newgunMode);
    void resetColourMasks();
    void clearColourMasks();
    
  private:
    CRGB _leds[NUM_LEDS];
    GunMode* _gunMode;
};


#endif
