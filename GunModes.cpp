/*
  GunModes.h - Defines the modes of the gun
*/

#include <Arduino.h>
#include "Buttons.h"
#include "Easing.h"
#include "GunModes.h"

#define DEBUG_MENU_SIZE 4;

/*
 * GunMode (stub methods)
 */
 
void GunMode::onEnter(Gun gun) {
  gun.clearColourMasks();
}

GunMode* GunMode::onBarrelSelected(Gun gun, int barrel) {
  return NULL;
}
 
GunMode* GunMode::onTriggerPressed(Gun gun) {
  return NULL;
}

GunMode* GunMode::onTriggerHeld(Gun gun, int holdType) {
  if (holdType == HOLD_TYPE_MEDIUM) {
    return DebugMenuGunMode::getInstance();
  }
  return NULL;
}

GunMode* GunMode::onTriggerReleased(Gun gun, int holdType) {
  return NULL;
}

/*
 * DebugMenuGunMode
 */
 
void DebugMenuGunMode::onEnter(Gun gun) {
  gun.resetColourMasks();
}

void DebugMenuGunMode::tick(Gun gun) {
  int index = _selection;
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    gun._barrels[barrel]->debugTick(index);
    index -= 4;
  }
}

GunMode* DebugMenuGunMode::onTriggerHeld(Gun gun, int holdType) {
  if (holdType == HOLD_TYPE_VERY_LONG) {
    return ActiveGunMode::getInstance(); 
  }
  return NULL;
}

GunMode* DebugMenuGunMode::onTriggerReleased(Gun gun, int holdType) {
  if (holdType < HOLD_TYPE_MEDIUM) {
    _selection = (_selection + 1) % DEBUG_MENU_SIZE;
  } else {
    switch (_selection) {
      case 0:
        return PartyGunMode::getInstance();
        
      case 1:
        return RainbowGunMode::getInstance();
        
      case 2:
        return TranquilityGunMode::getInstance();
        
      case 3:
        return CylonGunMode::getInstance();
    }
  }
  return NULL;
}

/*
 * ActiveGunMode
 */
 
void ActiveGunMode::onEnter(Gun gun) {
  gun.resetColourMasks();
}
 
void ActiveGunMode::tick(Gun gun) {
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    gun._barrels[barrel]->tick();
  }
}

GunMode* ActiveGunMode::onTriggerPressed(Gun gun) {
  Serial.print("Trigger pressed: ");
  Serial.println(gun._currentBarrel);
  gun._barrels[gun._currentBarrel]->triggerAction();
  return NULL;
}

GunMode* ActiveGunMode::onTriggerHeld(Gun gun, int holdType) {
  if (holdType == HOLD_TYPE_VERY_LONG) {
    return DebugMenuGunMode::getInstance();
  }
  return NULL;
}

/*
 * CylonGunMode
 */
 
void CylonGunMode::onEnter(Gun gun) {
  gun.resetColourMasks();
}

void CylonGunMode::tick(Gun gun) {
  int totalFrames = FRAMES_PER_SECOND * CYLON_DURATION;
  if (_frame >= totalFrames) {
    _frame = 0;
  }
  
  float journeyDuration = (float) totalFrames / 2;

  int translatedFrame = _frame;
  if (translatedFrame > journeyDuration) {
    translatedFrame = (totalFrames - _frame);
  }

  float eyePosition = Easing::easeInOutQuad((float) translatedFrame, 0.0, 1.0, journeyDuration);

  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    gun._barrels[barrel]->cylonTick(eyePosition);
  }

  _frame++;
}

/*
 * RainbowGunMode
 */

void RainbowGunMode::tick(Gun gun) {
  int totalFrames = FRAMES_PER_SECOND * RAINBOW_DURATION;
  if (_frame >= totalFrames) {
    _frame = 0;
  }
  
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    float seed = (float) _frame / totalFrames + (float) barrel / (BARREL_MAX + 1);
    gun._barrels[barrel]->rainbowTick(seed);
  }

  _frame++;
}

/*
 * TranquilityGunMode
 */

void TranquilityGunMode::tick(Gun gun) {
  int totalFrames = FRAMES_PER_SECOND * TRANQUILITY_DURATION;
  if (_frame >= totalFrames) {
    _frame = 0;
  }
  
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    float seed = (float) _frame / totalFrames;
    gun._barrels[barrel]->tranquilityTick(seed);
  }

  _frame++;
}

/*
 * PartyGunMode
 */

void PartyGunMode::tick(Gun gun) {
  int totalFrames = FRAMES_PER_SECOND * PARTY_DURATION;
  if (_frame >= totalFrames) {
    _frame = 0;
  }
  
  for (int barrel = 0; barrel <= BARREL_MAX; barrel++) {
    float seed = (float) _frame / totalFrames + (float) barrel / (BARREL_MAX + 1);
    if (seed > 1.0) {
      seed -= 1.0;
    }
    
    gun._barrels[barrel]->partyTick(seed);
  }

  _frame++;
}

