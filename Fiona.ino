#include "Barrel.h"
#include "Buttons.h"
#include "Consts.h"
#include "Gun.h"

/*
 * Variables
 */
Gun gun;

unsigned long triggerHoldStart = 0;
bool triggerHeld = false;
int triggerHoldType = 0;
bool triggerResolved = false;

void setup() { 
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(PIN_TRIGGER, INPUT);

  // For some reason this currently stops the memory allocation issues
  FastLED.show();
}

void loop() {
  long unsigned loopStart = millis();
  
  checkTrigger();

  gun.tick();
  
  FastLED.show();

  long unsigned loopDuration = millis() - loopStart;

  delay(1000/FRAMES_PER_SECOND - loopDuration);
}

void checkTrigger() {
  int triggerState = digitalRead(PIN_TRIGGER);
  if (triggerState == HIGH) {
    if (!triggerHeld) {
      triggerHeld = true;
      triggerHoldStart = millis();
      if (gun.triggerPressed()) {
        triggerResolved = true;
      }
    } else if (!triggerResolved) {
      int holdType = Buttons::getHoldType(millis() - triggerHoldStart);
      if (holdType == triggerHoldType) {
        triggerHoldType = holdType;
        if (gun.triggerHeld(holdType)) {
          triggerResolved = true;
        }
      }
    }
  } else if (triggerHeld) {
    gun.triggerReleased(Buttons::getHoldType(millis() - triggerHoldStart));
    triggerHeld = false;
    triggerResolved = false;
  }
}

