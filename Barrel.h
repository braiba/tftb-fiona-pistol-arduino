/*
  Barrel.h - Defines a barrel for Fiona's gun
*/
#ifndef Barrel_h
#define Barrel_h

#include "Arduino.h"
#include "FastLED.h"

#define LED_MUZZLE 0 
#define LED_FRONT 1
#define LED_MIDDLE 2
#define LED_BACK 3
#define LED_MAX 3

#define BARREL_MODE_CHARGED 0
#define BARREL_MODE_FIRING 1
#define BARREL_MODE_DEAD 2
#define BARREL_MODE_CHARGING 3

#define CHARGED_DURATION 5 // seconds
#define CHARGED_INTENSITY_MIN 0.1
#define CHARGED_INTENSITY_MAX 0.6

class Barrel
{
  public:
    Barrel(int reedPin, CRGB* barrelLed, CRGB* frontLed, CRGB* middleLed, CRGB* backLed);
    void setColourMask(float red, float green, float blue);
    void tick();
    void cylonTick(float eyePosition);
    void rainbowTick(float seed);
    void tranquilityTick(float seed);
    void partyTick(float seed);
    void debugTick(int selectedLed);
  private:
    int _reedPin;
    CRGB* _leds[4];
    float _colourMask[3];
    int _frame;
    int _mode;
    void setLed(int led, int red, int green, int blue);
    void setLedX(int led, int intensity);
    void setLedRot(int led, int rot);
    void chargedTick();
    float getChargedFrameIntensity(int frame, int totalFrames);
};

#endif
