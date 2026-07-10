#include "light.h"

#include "config.h"

void LightController::begin() {
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, LOW);
  lightOn_ = false;
}

void LightController::update(float relativePitchDeg) {
  if (!lightOn_ && relativePitchDeg >= config::kLightOnAngleDeg) {
    lightOn_ = true;
  } else if (lightOn_ && relativePitchDeg <= config::kLightOffAngleDeg) {
    lightOn_ = false;
  }

  digitalWrite(pin_, lightOn_ ? HIGH : LOW);
}
