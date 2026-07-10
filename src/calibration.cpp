#include "imu.h"

#include "config.h"
#include "logger.h"

void IMU::calibrate() {
  LOG(F("Calibrating IMU... keep bike still"));

  const uint32_t startTime = millis();
  float sumPitch = 0.0f;
  uint32_t sampleCount = 0;

  while ((millis() - startTime) < config::kCalibrationDurationMs) {
    float absolutePitchDeg = 0.0f;
    if (readAbsolutePitchDeg(absolutePitchDeg)) {
      sumPitch += absolutePitchDeg;
      ++sampleCount;
    }

    delay(config::kCalibrationSampleDelayMs);
  }

  if (sampleCount > 0) {
    zeroPitchDeg_ = sumPitch / static_cast<float>(sampleCount);
  }

  relativePitchDeg_ = 0.0f;
  LOG(F("Calibration complete"));
  LOG(zeroPitchDeg_);
}
