/*
  Gun.cpp - Defines a Fiona's gun
*/

#include "Arduino.h"
#include "Barrel.h"
#include "FastLED.h"
#include "Gun.h"
#include "GunModes.h"

Gun::Gun()
{
  Serial.print("Gun::Gun()");
  FastLED.addLeds<WS2812, PIN_DATA, GRB>(_leds, NUM_LEDS);
  
  Barrel redBarrel = Barrel(
    PIN_REED_BARREL_RED,
    &_leds[LED_RED_MUZZLE],
    &_leds[LED_RED_FRONT],
    &_leds[LED_RED_MIDDLE],
    &_leds[LED_RED_BACK]
  );
  _barrels[BARREL_RED] = &redBarrel;
  
  Barrel greenBarrel = Barrel(
    PIN_REED_BARREL_GREEN,
    &_leds[LED_GREEN_MUZZLE],
    &_leds[LED_GREEN_FRONT],
    &_leds[LED_GREEN_MIDDLE],
    &_leds[LED_GREEN_BACK]
  );
  _barrels[BARREL_GREEN] = &greenBarrel;
  
  Barrel blueBarrel = Barrel(
    PIN_REED_BARREL_BLUE,
    &_leds[LED_BLUE_MUZZLE],
    &_leds[LED_BLUE_FRONT],
    &_leds[LED_BLUE_MIDDLE],
    &_leds[LED_BLUE_BACK]
  );
  _barrels[BARREL_BLUE] = &blueBarrel;

  ActiveGunMode *gunMode;
  setGunMode(gunMode->getInstance());
}

void Gun::setGunMode(GunMode* newGunMode)
{
  _gunMode = newGunMode;
}

void Gun::tick()
{
  _gunMode->tick(*this);
}

boolean Gun::triggerPressed()
{
  GunMode* newGunMode = _gunMode->triggerPressed(*this);
  if (newGunMode) {
    _gunMode = newGunMode;
    return true;
  }
  return false;
}

boolean Gun::triggerHeld(int holdType)
{
  GunMode* newGunMode = _gunMode->triggerHeld(*this, holdType);
  if (newGunMode) {
    _gunMode = newGunMode;
    return true;
  }
  return false;
}

boolean Gun::triggerReleased(int holdType)
{
  GunMode* newGunMode = _gunMode->triggerReleased(*this, holdType);
  if (newGunMode) {
    _gunMode = newGunMode;
    return true;
  }
  return false;
}

void Gun::resetColourMasks() 
{
  _barrels[BARREL_RED]->setColourMask(1.0, 0.0, 0.0);
  _barrels[BARREL_GREEN]->setColourMask(0.0, 1.0, 0.0);
  _barrels[BARREL_BLUE]->setColourMask(0.0, 0.0, 1.0);
}

void Gun::clearColourMasks() 
{
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    _barrels[barrel]->setColourMask(1.0, 1.0, 1.0);
  }
}

