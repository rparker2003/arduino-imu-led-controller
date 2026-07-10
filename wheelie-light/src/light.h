#pragma once

#include <Arduino.h>

class LightController {
 public:
  explicit LightController(uint8_t pin) : pin_(pin) {}

  void begin();
  void update(float relativePitchDeg);

  bool isOn() const { return lightOn_; }

 private:
  uint8_t pin_;
  bool lightOn_ = false;
};
