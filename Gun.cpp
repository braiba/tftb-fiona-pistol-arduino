/*
  Gun.cpp - Defines a Fiona's gun
*/

#include "Arduino.h"
#include "Barrel.h"
#include "Buttons.h"
#include "FastLED.h"
#include "Gun.h"
#include "GunModes.h"

Gun::Gun()
{
  pinMode(PIN_TRIGGER, INPUT);
  FastLED.addLeds<WS2812, PIN_DATA, GRB>(_leds, NUM_LEDS);
  
  _barrels[BARREL_RED] = new Barrel(
    PIN_REED_BARREL_RED,
    &_leds[LED_RED_MUZZLE],
    &_leds[LED_RED_FRONT],
    &_leds[LED_RED_MIDDLE],
    &_leds[LED_RED_BACK]
  );
  
  _barrels[BARREL_GREEN] = new Barrel(
    PIN_REED_BARREL_GREEN,
    &_leds[LED_GREEN_MUZZLE],
    &_leds[LED_GREEN_FRONT],
    &_leds[LED_GREEN_MIDDLE],
    &_leds[LED_GREEN_BACK]
  );
  
  _barrels[BARREL_BLUE] = new Barrel(
    PIN_REED_BARREL_BLUE,
    &_leds[LED_BLUE_MUZZLE],
    &_leds[LED_BLUE_FRONT],
    &_leds[LED_BLUE_MIDDLE],
    &_leds[LED_BLUE_BACK]
  );
  
  // For some reason this currently stops the memory allocation issues
  FastLED.show();

  setGunMode(ActiveGunMode::getInstance());
}

void Gun::setGunMode(GunMode* newGunMode)
{
  _gunMode = newGunMode;
  newGunMode->onEnter(*this);
}

void Gun::tick()
{
  checkTrigger();
  _gunMode->tick(*this);
  
  FastLED.show();
}

void Gun::checkTrigger() {
  int triggerState = digitalRead(PIN_TRIGGER);
  if (triggerState == HIGH) {
    if (_triggerHeld == false) {
      _triggerHeld = true;
      _triggerHoldStart = millis();
      _triggerHoldType = HOLD_TYPE_NONE;
      if (onTriggerPressed()) {
        _triggerResolved = true;
      }
    } else if (_triggerResolved == false) {
      int holdType = Buttons::getHoldType(millis() - _triggerHoldStart);
      if (holdType != _triggerHoldType) {
        _triggerHoldType = holdType;
        if (onTriggerHeld(holdType)) {
          _triggerResolved = true;
        }
      }
    }
  } else if (_triggerHeld) {
    if (_triggerResolved == false) {
      onTriggerReleased(Buttons::getHoldType(millis() - _triggerHoldStart));
    }
    _triggerHeld = false;
    _triggerResolved = false;
  }
}

boolean Gun::onTriggerPressed()
{
  GunMode* newGunMode = _gunMode->onTriggerPressed(*this);
  if (newGunMode != NULL) {
    setGunMode(newGunMode);
    return true;
  }
  return false;
}

boolean Gun::onTriggerHeld(int holdType)
{
  GunMode* newGunMode = _gunMode->onTriggerHeld(*this, holdType);
  if (newGunMode != NULL) {
    setGunMode(newGunMode);
    return true;
  }
  return false;
}

boolean Gun::onTriggerReleased(int holdType)
{
  GunMode* newGunMode = _gunMode->onTriggerReleased(*this, holdType);
  if (newGunMode != NULL) {
    setGunMode(newGunMode);
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

