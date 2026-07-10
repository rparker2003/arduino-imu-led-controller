#pragma once

#include <Arduino.h>

class IMU {
 public:
  bool begin();
  void calibrate();
  bool update();

  float getPitch() const { return relativePitchDeg_; }
  float getZeroPitch() const { return zeroPitchDeg_; }

 private:
  bool readAccelRaw(int16_t& ax, int16_t& ay, int16_t& az) const;
  bool readAbsolutePitchDeg(float& pitchDeg) const;
  static float computePitchFromAccel(int16_t ax, int16_t ay, int16_t az);

  float zeroPitchDeg_ = 0.0f;
  float relativePitchDeg_ = 0.0f;
};
