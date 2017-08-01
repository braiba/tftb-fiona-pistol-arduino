/*
  Barrel.cpp - Defines a barrel for Fiona's gun
*/

#include "Barrel.h"

Barrel::Barrel(int reedPin, CRGB* muzzleLed, CRGB* frontLed, CRGB* middleLed, CRGB* backLed)
{
  _reedPin = reedPin;
  _leds[BARREL_LED_MUZZLE] = muzzleLed;
  _leds[BARREL_LED_FRONT] = frontLed;
  _leds[BARREL_LED_MIDDLE] = middleLed;
  _leds[BARREL_LED_BACK] = backLed;
  
  _frame = 0;
  _barrelMode = BARREL_MODE_CHARGED;
  _colourMask[0] = 1.0;
  _colourMask[1] = 1.0;
  _colourMask[2] = 1.0;
  
  pinMode(reedPin, INPUT);
}

/*
 * Public methods
 */

// Active mode

void Barrel::tick()
{
  switch (_barrelMode) {
    case BARREL_MODE_CHARGED:
      chargedTick();
      break;
      
    case BARREL_MODE_FIRING:
      firingTick();
      break;
      
    case BARREL_MODE_DEAD:
      deadTick();
      break;
      
    case BARREL_MODE_CHARGING:
      chargingTick();
      break;
  }
}

void Barrel::triggerAction()
{
  switch (_barrelMode) {
    case BARREL_MODE_CHARGED:
      setBarrelMode(BARREL_MODE_FIRING);
      break;
      
    case BARREL_MODE_DEAD:
      setBarrelMode(BARREL_MODE_CHARGING);
      break;
  }
}

// Cylon mode

void Barrel::cylonTick(float eyePosition)
{
  float ledAwareness = 0.3;
  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    float ledPosition = (float) led / BARREL_LED_MAX;
    float ledDistance = abs(ledPosition - eyePosition);
    if (ledDistance < ledAwareness) {
      setLedX(led, ((ledAwareness - ledDistance) / ledAwareness) * 255);
    } else {
      setLedX(led, 0);
    }
  }
}

// Rainbow mode

void Barrel::rainbowTick(float seed)
{
  int rot = 360 * seed;
  
  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedRot(led, rot);
  }
}

// Tranquility mode

void Barrel::tranquilityTick(float seed)
{
  int rot = 360 * seed;
  
  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedRot(led, rot);
  }
}

// Party mode

void Barrel::partyTick(float seed)
{
  int rot = 360 * seed;
  
  setLedRot(BARREL_LED_MUZZLE, rot - 80);
  setLedRot(BARREL_LED_FRONT, rot - 40);
  setLedRot(BARREL_LED_MIDDLE, rot);
  setLedRot(BARREL_LED_BACK, rot + 40);
}

// Debug mode

void Barrel::debugTick(int selectedLed)
{
  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedX(led, (led == selectedLed ? 255 : 0));
  }
}

// Miscelleaneous methods

void Barrel::setColourMask(float red, float green, float blue)
{
  _colourMask[0] = red;
  _colourMask[1] = green;
  _colourMask[2] = blue;
}

/*
 * Private methods
 */

// Active mode

void Barrel::setBarrelMode(int barrelMode)
{
  _barrelMode = barrelMode;
  _frame = 0;
}

void Barrel::chargedTick() 
{ 
  int totalFrames =  CHARGED_DURATION * FRAMES_PER_SECOND;
  float intensity = getChargedFrameIntensity(_frame, totalFrames);
  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedX(led, intensity * 255);
  }
  setLedX(BARREL_LED_MUZZLE, (CHARGED_INTENSITY_MIN + CHARGED_INTENSITY_MAX) / 2);
    
  _frame++;
  if (_frame >= totalFrames) {
    _frame = 0;
  }
}

float Barrel::getChargedFrameIntensity(int frame, int totalFrames)
{
  float journeyDuration = (float) totalFrames / 2;

  while (frame > totalFrames) {
    frame -= totalFrames;
  }
  
  int translatedFrame = frame;
  if (translatedFrame > journeyDuration) {
    translatedFrame =  (totalFrames - frame);
  }
  
  return Easing::easeInOutQuad((float) translatedFrame, CHARGED_INTENSITY_MIN, CHARGED_INTENSITY_MAX - CHARGED_INTENSITY_MIN, journeyDuration);
}

void Barrel::firingTick()
{
  int totalFrames = FIRING_DURATION * FRAMES_PER_SECOND;
  float intensity = Easing::easeInOutQuad((float) _frame, CHARGED_INTENSITY_MAX, -CHARGED_INTENSITY_MAX, totalFrames);

  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedX(led, intensity * 255);
  }
  
  _frame++;
  if (_frame == totalFrames) {
    setBarrelMode(BARREL_MODE_DEAD);
  }
}

void Barrel::deadTick()
{
  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedX(led, 0);
  }
}

void Barrel::chargingTick()
{
  int totalFrames = CHARGED_DURATION * FRAMES_PER_SECOND;
  float intensity = Easing::easeInOutQuad((float) _frame, 0, CHARGED_INTENSITY_MIN, totalFrames);

  for (int led = 0; led <= BARREL_LED_MAX; led++) {
    setLedX(led, intensity * 255);
  }
  setLedX(BARREL_LED_MUZZLE, 0);
  
  _frame++;
  if (_frame == totalFrames) {
    setBarrelMode(BARREL_MODE_CHARGED);
  }
}

// Miscellaneous methods

void Barrel::setLed(int led, int red, int green, int blue)
{
  _leds[led]->red = red * _colourMask[0];
  _leds[led]->green = green * _colourMask[1];
  _leds[led]->blue = blue * _colourMask[2];
}

void Barrel::setLedX(int led, int intensity)
{
  setLed(led, intensity, intensity, intensity);
}

void Barrel::setLedRot(int led, int rot)
{
  float r, g, b;
  while (rot < 0) {
    rot += 360;
  }
  while (rot >= 360) {
    rot -= 360;
  }

  if (rot >= 0 && rot < 60 )
  {
    r = 1.0;
    g = float(rot) / 60.0;
    b = 0.0;
  }
  if (rot >= 60 && rot < 120 )
  {
    r = (120.0 - float(rot)) / 60.0;
    g = 1.0;
    b = 0.0;
  }
  if (rot >= 120 && rot < 180 )
  {
    r = 0.0;
    g = 1.0;
    b = (float(rot) - 120.0) / 60;
  }
  if (rot >= 180 && rot < 240 )
  {
    r = 0.0;
    g = (240.0 - float(rot)) / 60.0;
    b = 1.0;
  }
  if (rot >= 240 && rot < 300 )
  {
    r = (float(rot) - 240.0) / 60;
    g = 0.0;
    b = 1.0;
  }
  if (rot >= 300 && rot < 360 )
  {
    r = 1.0;
    g = 0.0;
    b = (360.0 - float(rot)) / 60;
  }
  
  setLed(led, r * 255, g * 255, b * 255);
}

