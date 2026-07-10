#pragma once

#include <Arduino.h>
#include "config.h"

namespace Logger {
#if DEBUG
inline void begin(unsigned long baud = 115200) {
  Serial.begin(baud);
  const uint32_t start = millis();
  while (!Serial && (millis() - start < 1500)) {
  }
}

template <typename T>
inline void log(const T& value) {
  Serial.println(value);
}
#else
inline void begin(unsigned long = 115200) {}

template <typename T>
inline void log(const T&) {}
#endif

}  // namespace Logger

#define LOG(value) Logger::log(value)
