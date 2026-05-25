#pragma once

#include <Arduino.h>

#if defined(RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_LCD_349) && \
    defined(RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_AMOLED_241)
#error "Select only one RSVP board target."
#endif

#if !defined(RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_LCD_349) && \
    !defined(RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_AMOLED_241)
#define RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_LCD_349 1
#endif

namespace BoardConfig {

enum class UiOrientation : uint8_t {
  Landscape = 0,
  LandscapeFlipped,
  Portrait,
  PortraitFlipped,
};

enum class DisplayDriverKind : uint8_t {
  Axs15231b = 0,
  Rm690b0,
};

enum class TouchControllerKind : uint8_t {
  Axs15231b = 0,
  Ft6336,
};

enum class StorageBusKind : uint8_t {
  SdMmc1Bit = 0,
  SdSpi,
};

enum class PowerManagerKind : uint8_t {
  Tca9554 = 0,
  Axp2101,
  DirectGpioBatteryHold,
};

#if defined(RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_AMOLED_241)
#include "board/profiles/WaveshareEsp32S3TouchAmoled241Profile.h"
#elif defined(RSVP_BOARD_WAVESHARE_ESP32S3_TOUCH_LCD_349)
#include "board/profiles/WaveshareEsp32S3TouchLcd349Profile.h"
#else
#error "Unsupported RSVP board target."
#endif

struct BatteryStatus {
  bool present = false;
  float voltage = 0.0f;
  uint8_t percent = 0;
};

void begin();
void lightSleepUntilBootButton();
void holdBacklightOffForDeepSleep();
bool readBatteryStatus(BatteryStatus &status);
bool releaseBatteryPowerHold();

}  // namespace BoardConfig
