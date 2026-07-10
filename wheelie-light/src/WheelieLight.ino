#include "config.h"
#include "imu.h"
#include "light.h"
#include "logger.h"

namespace {
IMU gImu;
LightController gLight(config::kLightPin);
uint32_t gLastLoopMs = 0;
}

void setup() {
  Logger::begin();
  LOG(F("Wheelie light boot"));

  while (!gImu.begin()) {
    LOG(F("Retrying IMU init..."));
    delay(500);
  }
  gLight.begin();
  gImu.calibrate();

  gLastLoopMs = millis();
}

void loop() {
  const uint32_t now = millis();
  if ((now - gLastLoopMs) < config::kLoopIntervalMs) {
    return;
  }
  gLastLoopMs = now;

  if (gImu.update()) {
    const float pitch = gImu.getPitch();
    gLight.update(pitch);
    LOG(pitch);
  }
}
