#include "imu.h"

#include <Wire.h>

#include "config.h"
#include "logger.h"

namespace {
constexpr uint8_t kMpu6050PowerMgmtRegister = 0x6B;
constexpr uint8_t kMpu6050AccelStartRegister = 0x3B;
}

bool IMU::begin() {
  Wire.begin();
  Wire.setClock(400000UL);

  Wire.beginTransmission(config::kMpu6050Address);
  Wire.write(kMpu6050PowerMgmtRegister);
  Wire.write(0x00);  // Wake up MPU6050
  if (Wire.endTransmission() != 0) {
    LOG(F("IMU init failed"));
    return false;
  }

  LOG(F("IMU ready"));
  return true;
}

bool IMU::update() {
  float absolutePitchDeg = 0.0f;
  if (!readAbsolutePitchDeg(absolutePitchDeg)) {
    return false;
  }

  relativePitchDeg_ = absolutePitchDeg - zeroPitchDeg_;
  return true;
}

bool IMU::readAbsolutePitchDeg(float& pitchDeg) const {
  int16_t ax = 0;
  int16_t ay = 0;
  int16_t az = 0;

  if (!readAccelRaw(ax, ay, az)) {
    return false;
  }

  pitchDeg = computePitchFromAccel(ax, ay, az);
  return true;
}

bool IMU::readAccelRaw(int16_t& ax, int16_t& ay, int16_t& az) const {
  Wire.beginTransmission(config::kMpu6050Address);
  Wire.write(kMpu6050AccelStartRegister);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  constexpr uint8_t bytesToRead = 6;
  uint8_t received = Wire.requestFrom(static_cast<int>(config::kMpu6050Address),
                                      static_cast<int>(bytesToRead),
                                      static_cast<int>(true));
  if (received != bytesToRead) {
    return false;
  }

  ax = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
  ay = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
  az = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
  return true;
}

float IMU::computePitchFromAccel(int16_t ax, int16_t ay, int16_t az) {
  const float axf = static_cast<float>(ax);
  const float ayf = static_cast<float>(ay);
  const float azf = static_cast<float>(az);
  const float denominator = sqrtf((ayf * ayf) + (azf * azf));
  return atan2f(axf, denominator) * 180.0f / PI;
}
