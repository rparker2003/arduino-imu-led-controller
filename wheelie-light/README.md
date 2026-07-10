# Automatic Wheelie Light Firmware (Arduino Pro Micro)

Embedded firmware for an Arduino Pro Micro (ATmega32U4) that reads pitch from an MPU6050 and controls a 12V wheelie LED through an IRLZ44N MOSFET.

## Project Purpose

When the bike reaches a wheelie angle, the wheelie light automatically turns on. Hysteresis keeps the light stable and prevents flicker near threshold angles.

## Hardware

- Arduino Pro Micro (ATmega32U4)
- MPU6050 (GY-521)
- IRLZ44N N-channel MOSFET (low-side switching)
- 12V to 5V buck converter (Arduino power)
- 12V LED light

## Wiring Overview

- Buck converter 5V/GND -> Pro Micro VCC/GND
- MPU6050 -> Pro Micro I2C (SDA/SCL) + 5V/GND
- Pro Micro digital pin D9 -> MOSFET gate
- LED negative -> MOSFET drain
- MOSFET source -> bike ground
- LED positive -> bike 12V

## Firmware Behavior

### Startup Calibration

1. Wait ~3 seconds while collecting many IMU samples.
2. Average measured pitch during that window.
3. Store averaged pitch as startup zero reference.
4. Enter normal operation.

### Normal Operation (~100 Hz)

- Read IMU acceleration.
- Compute pitch relative to calibrated zero.
- Turn light **ON** at `>= 30°`.
- Turn light **OFF** at `<= 10°`.

## PlatformIO Setup

1. Install PlatformIO Core:
   - `pip install platformio`
2. From the repository root, move into the firmware folder:
   - `cd wheelie-light`
3. Build:
   - `pio run`
4. Upload:
   - `pio run -t upload`
5. Open serial monitor:
   - `pio device monitor -b 115200`

`platformio.ini` is configured for Arduino Pro Micro 5V/16MHz (`sparkfun_promicro16`).

## Configuration

Edit `wheelie-light/src/config.h`:

- `DEBUG` (enable/disable serial logging at compile time)
- `kLightPin`
- `kLightOnAngleDeg`
- `kLightOffAngleDeg`
- `kCalibrationDurationMs`
- `kCalibrationSampleDelayMs`
- `kLoopIntervalMs`

You can also override `DEBUG` in PlatformIO build flags (`platformio.ini`).

## Logging

Use `LOG(value)` from `logger.h`.

- When `DEBUG` is `1`, logs are sent over Serial.
- When `DEBUG` is `0`, logging compiles to no-ops.

## Architecture

```text
wheelie-light/
├── README.md
├── src/
│   ├── WheelieLight.ino
│   ├── config.h
│   ├── imu.h
│   ├── imu.cpp
│   ├── calibration.cpp
│   ├── light.h
│   ├── light.cpp
│   └── logger.h
├── docs/
└── lib/
```

The main sketch remains small and orchestration-focused, with IMU handling and light logic separated into modules.

## CI / GitHub Actions

CI build is defined in:

- `.github/workflows/platformio-ci.yml`

It installs PlatformIO and runs `pio run` inside `wheelie-light` on each push and pull request.

## PlatformIO Compatibility Notes

- `platformio.ini` was added in `wheelie-light/` so PlatformIO uses the existing `src/` directory without reorganizing modules.
- `MPU6050_tockn` is declared in `lib_deps` to make dependency resolution reproducible for future IMU implementation changes, while current firmware remains functionally unchanged.

## Future Improvements

Planned extension points are intentionally supported by the current module split:

- EEPROM-backed configuration
- Calibration button
- Adjustable angles via runtime config
- Flashing/pattern modes
- Brake light mode
- Bluetooth/USB serial config protocol
- OLED status display
- Additional IMU-derived behaviors
