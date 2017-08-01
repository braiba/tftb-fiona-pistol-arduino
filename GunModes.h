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
    virtual void onEnter(Gun gun);
    virtual GunMode* onBarrelSelected(Gun gun, int barrel);
    virtual GunMode* onTriggerPressed(Gun gun);
    virtual GunMode* onTriggerHeld(Gun gun, int holdType);
    virtual GunMode* onTriggerReleased(Gun gun, int holdType);
    virtual void tick(Gun gun) = 0;
};

class DebugMenuGunMode : public GunMode {
  public:
    static DebugMenuGunMode* getInstance()
    {
      static DebugMenuGunMode singleton;
      return &singleton;
    }
    
    void onEnter(Gun gun);
    GunMode* onTriggerHeld(Gun gun, int holdType);
    GunMode* onTriggerReleased(Gun gun, int holdType);
    void tick(Gun gun);
    
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
    
    void onEnter(Gun gun);
    void tick(Gun gun);
    GunMode* onTriggerPressed(Gun gun);
    GunMode* onTriggerHeld(Gun gun, int holdType);

  private:
    ActiveGunMode() {}
};

class CylonGunMode : public GunMode {
  public:
    static CylonGunMode* getInstance()
    {
      static CylonGunMode singleton;
      return &singleton;
    }

    void onEnter(Gun gun);
    void tick(Gun gun);
    
  private:
    CylonGunMode() {}
    int _frame = 0;
};

class RainbowGunMode : public GunMode {
  public:
    static RainbowGunMode* getInstance()
    {
      static RainbowGunMode singleton;
      return &singleton;
    }

    void tick(Gun gun);
    
  private:
    RainbowGunMode() {}
    int _frame = 0;
};

class TranquilityGunMode : public GunMode {
  public:
    static TranquilityGunMode* getInstance()
    {
      static TranquilityGunMode singleton;
      return &singleton;
    }

    void tick(Gun gun);
    
  private:
    TranquilityGunMode() {}
    int _frame = 0;
};

class PartyGunMode : public GunMode {
  public:
    static PartyGunMode* getInstance()
    {
      static PartyGunMode singleton;
      return &singleton;
    }
    
    void tick(Gun gun);

  private:
    PartyGunMode() {}
    int _frame = 0;
};

#endif

