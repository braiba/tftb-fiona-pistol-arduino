/*
  GunModes.h - Defines the modes of the gun
*/
#ifndef GunModes_h
#define GunModes_h

#include "Gun.h"

#define CYLON_DURATION 0.9 // seconds

#define RAINBOW_DURATION 5 // seconds

#define TRANQUILITY_DURATION 15 // seconds

#define PARTY_DURATION 0.4 // seconds

class Gun;

class GunMode {
  public:
    virtual void tick(Gun gun) = 0;
    GunMode* triggerPressed(Gun gun);
    GunMode* triggerHeld(Gun gun, int holdType);
    GunMode* triggerReleased(Gun gun, int holdType);
};

class DebugMenuGunMode : public GunMode {
  public:
    static DebugMenuGunMode* getInstance()
    {
      static DebugMenuGunMode singleton;
      return &singleton;
    }
    
    void tick(Gun gun);
    GunMode* triggerHeld(Gun gun, int holdType);
    GunMode* triggerReleased(Gun gun, int holdType);
    
  private:
    DebugMenuGunMode() {}
    
    int _selection = 0;
};

class ActiveGunMode : public GunMode {
  public:
    static ActiveGunMode* getInstance()
    {
      static ActiveGunMode singleton;
      return &singleton;
    }
    
    void tick(Gun gun);
    GunMode* triggerHeld(Gun gun, int holdType);
    GunMode* triggerReleased(Gun gun, int holdType);

  private:
    ActiveGunMode() {}
};

class CylonGunMode : public GunMode {
  public:
    static CylonGunMode& getInstance();

    void tick(Gun gun);
    
  private:
    CylonGunMode() {}
    int _frame = 0;
};

class RainbowGunMode : public GunMode {
  public:
    static RainbowGunMode& getInstance();

    void tick(Gun gun);
    
  private:
    RainbowGunMode() {}
    int _frame = 0;
};

class TranquilityGunMode : public GunMode {
  public:
    static TranquilityGunMode& getInstance();

    void tick(Gun gun);
    
  private:
    TranquilityGunMode() {}
    int _frame = 0;
};

class PartyGunMode : public GunMode {
  public:
    static PartyGunMode& getInstance();
    
    void tick(Gun gun);

  private:
    PartyGunMode() {}
    int _frame = 0;
};

#endif

