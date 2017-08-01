#include "Consts.h"
#include "Gun.h"

Gun* gun;

void setup() { 
  Serial.begin(9600);
  randomSeed(analogRead(0));

  Serial.println("Root setup");
  
  gun = new Gun();
}

void loop() {
  long unsigned loopStart = millis();

  gun->tick();

  long unsigned loopDuration = millis() - loopStart;

  delay(1000/FRAMES_PER_SECOND - loopDuration);
}

