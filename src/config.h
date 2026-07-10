#pragma once

#include <Arduino.h>

#ifndef DEBUG
#define DEBUG 1
#endif

namespace config {
constexpr uint8_t kLightPin = 9;
constexpr uint8_t kMpu6050Address = 0x68;

constexpr float kLightOnAngleDeg = 30.0f;
constexpr float kLightOffAngleDeg = 10.0f;

constexpr uint32_t kCalibrationDurationMs = 3000;
constexpr uint16_t kCalibrationSampleDelayMs = 5;

constexpr uint16_t kLoopIntervalMs = 10;  // ~100 Hz
}  // namespace config
