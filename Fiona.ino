#include "Barrel.h"
#include "Easing.h"
#include "FastLED.h"

/*
 * Hardware consts
 */
#define NUM_LEDS 12
 
#define PIN_DATA 7

#define PIN_TRIGGER 11

#define PIN_REED_BARREL_RED 10
#define PIN_REED_BARREL_GREEN 9
#define PIN_REED_BARREL_BLUE 8

#define LED_RED_MUZZLE 1
#define LED_RED_FRONT 6
#define LED_RED_MIDDLE 7
#define LED_RED_BACK 8

#define LED_GREEN_MUZZLE 2
#define LED_GREEN_FRONT 9
#define LED_GREEN_MIDDLE 10
#define LED_GREEN_BACK 11

#define LED_BLUE_MUZZLE 0
#define LED_BLUE_FRONT 3
#define LED_BLUE_MIDDLE 4
#define LED_BLUE_BACK 5

#define LED_MAX 11

#define BARREL_RED 0
#define BARREL_GREEN 1
#define BARREL_BLUE 2
#define BARREL_MAX 2 // DEBUG

/*
 * Software consts
 */
#define FRAMES_PER_SECOND 30

#define MODE_ACTIVE 0
#define MODE_SPINNING 1
#define MODE_IDLE 2

#define MODE_CYLON 251
#define MODE_TRANQUILITY 252
#define MODE_RAINBOW 253
#define MODE_PARTY 254
#define MODE_DEBUG_MENU 255

#define CYLON_DURATION 0.9 // seconds

#define RAINBOW_DURATION 5 // seconds

#define TRANQUILITY_DURATION 15 // seconds

#define PARTY_DURATION 0.4 // seconds

#define HOLD_DURATION_TAP 0 // seconds
#define HOLD_DURATION_PRESS 0.2 // seconds
#define HOLD_DURATION_MEDIUM 0.5 // seconds
#define HOLD_DURATION_LONG 2 // seconds
#define HOLD_DURATION_DEBUG 3.5 // seconds

/*
 * Variables
 */
CRGB leds[NUM_LEDS];
Barrel* barrels[3];

int mode;
int activeBarrel = -1;
int frame = 0;
bool triggerHeld = true;
int triggerHoldDuration = 0;
int debugMenuSelected = 0;

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812, PIN_DATA, GRB>(leds, NUM_LEDS);
  pinMode(PIN_TRIGGER, INPUT);
  randomSeed(analogRead(0));

  Barrel redBarrel = Barrel(
    PIN_REED_BARREL_RED,
    &leds[LED_RED_MUZZLE],
    &leds[LED_RED_FRONT],
    &leds[LED_RED_MIDDLE],
    &leds[LED_RED_BACK]
  );
  barrels[BARREL_RED] = &redBarrel;
  
  Barrel greenBarrel = Barrel(
    PIN_REED_BARREL_GREEN,
    &leds[LED_GREEN_MUZZLE],
    &leds[LED_GREEN_FRONT],
    &leds[LED_GREEN_MIDDLE],
    &leds[LED_GREEN_BACK]
  );
  barrels[BARREL_GREEN] = &greenBarrel;
  
  Barrel blueBarrel = Barrel(
    PIN_REED_BARREL_BLUE,
    &leds[LED_BLUE_MUZZLE],
    &leds[LED_BLUE_FRONT],
    &leds[LED_BLUE_MIDDLE],
    &leds[LED_BLUE_BACK]
  );
  barrels[BARREL_BLUE] = &blueBarrel;

  initialiseMode(MODE_ACTIVE);

  // For some reason this currently stops the memory allocation issues
  FastLED.show();
}

void loop() {
  int triggerState = digitalRead(PIN_TRIGGER);
  if (triggerState == HIGH) {
    if (!triggerHeld) {
      triggerHeld = true;
      triggerHoldDuration = 0;
    } else {
      triggerHoldDuration++;
    }
  } else if (triggerHeld) {
    triggerReleased((float) triggerHoldDuration / FRAMES_PER_SECOND);
    triggerHeld = false;
  }
  
  switch (mode) {
    case MODE_ACTIVE:
      activeTick();
      break;
      
    case MODE_DEBUG_MENU:
      debugMenuTick();
      break;
      
    case MODE_CYLON:
      cylonTick();
      break;
      
    case MODE_RAINBOW:
      rainbowTick();
      break;
      
    case MODE_TRANQUILITY:
      tranquilityTick();
      break;
      
    case MODE_PARTY:
      partyTick();
      break;
  }

  frame++;
  
  FastLED.show();

  delay(1000/FRAMES_PER_SECOND);
}

void triggerReleased(float duration)
{
  if (duration > HOLD_DURATION_DEBUG) {
    if (mode != MODE_DEBUG_MENU) {  
      initialiseMode(MODE_DEBUG_MENU);
    } else {
      initialiseMode(MODE_ACTIVE);
    }
      return;
  }
  
  switch (mode) {
    case MODE_DEBUG_MENU:
      if (duration < HOLD_DURATION_MEDIUM) {
        debugMenuSelected = (debugMenuSelected + 1) % NUM_LEDS;
      } else {
        switch (debugMenuSelected) {
          case 0:
            initialiseMode(MODE_PARTY);
            break;
            
          case 1:
            initialiseMode(MODE_RAINBOW);
            break;
            
          case 2:
            initialiseMode(MODE_TRANQUILITY);
            break;
            
          case 3:
            initialiseMode(MODE_CYLON);
            break;
        }
      }
      break;
  }
}

void initialiseMode(int newMode)
{
  mode = newMode;
  frame = 0;

  switch (mode) {
    case MODE_RAINBOW:
    case MODE_TRANQUILITY:
    case MODE_PARTY:
      clearColourMasks();
      break;

    default:
      resetColourMasks();
      break;
  }
}

void activeTick() 
{
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    barrels[barrel]->tick();
  }
}

void debugMenuTick() 
{
  int index = debugMenuSelected;
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    barrels[barrel]->debugTick(index);
    index -= 4;
  }
}

void cylonTick() 
{
  int totalFrames = FRAMES_PER_SECOND * CYLON_DURATION;
  if (frame >= totalFrames) {
    frame = 0;
  }
  
  float journeyDuration = (float) totalFrames / 2;

  float translatedFrame = (float) frame;
  if (translatedFrame > journeyDuration) {
    translatedFrame = (float) (totalFrames - frame);
  }

  float eyePosition = Easing::easeInOutQuad(translatedFrame, 0.0, 1.0, journeyDuration);

  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    barrels[barrel]->cylonTick(eyePosition);
  }
}

void rainbowTick() 
{
  int totalFrames = FRAMES_PER_SECOND * RAINBOW_DURATION;
  if (frame >= totalFrames) {
    frame = 0;
  }
  
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    float seed = (float) frame / totalFrames + (float) barrel / (BARREL_MAX + 1);
    barrels[barrel]->rainbowTick(seed);
  }
}

void tranquilityTick() 
{
  int totalFrames = FRAMES_PER_SECOND * TRANQUILITY_DURATION;
  if (frame >= totalFrames) {
    frame = 0;
  }
  
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    float seed = (float) frame / totalFrames;
    barrels[barrel]->tranquilityTick(seed);
  }
}

void partyTick() 
{
  int totalFrames = FRAMES_PER_SECOND * PARTY_DURATION;
  if (frame >= totalFrames) {
    frame = 0;
  }
  
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    float seed = (float) frame / totalFrames + (float) barrel / (BARREL_MAX + 1);
    if (seed > 1.0) {
      seed -= 1.0;
    }
    
    barrels[barrel]->partyTick(seed);
  }
}

void resetColourMasks() 
{
  barrels[BARREL_RED]->setColourMask(1.0, 0.0, 0.0);
  barrels[BARREL_GREEN]->setColourMask(0.0, 1.0, 0.0);
  barrels[BARREL_BLUE]->setColourMask(0.0, 0.0, 1.0);
}

void clearColourMasks() 
{
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    barrels[barrel]->setColourMask(1.0, 1.0, 1.0);
  }
}

