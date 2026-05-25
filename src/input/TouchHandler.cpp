#include "input/TouchHandler.h"

#include <algorithm>
#include <Wire.h>

#include "board/BoardConfig.h"

namespace {

constexpr uint8_t kReadTouchCommand[] = {
    0xB5, 0xAB, 0xA5, 0x5A, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
};
constexpr uint32_t kPollIntervalMs = 20;
constexpr uint32_t kFailureBackoffMs = 250;
constexpr uint8_t kReleaseConfirmSamples = 2;
constexpr uint8_t kFt6336PointsReg = 0x02;

uint16_t clampDisplayX(uint16_t x) {
  return std::min<uint16_t>(x, static_cast<uint16_t>(BoardConfig::DISPLAY_WIDTH - 1));
}

uint16_t clampDisplayY(uint16_t y) {
  return std::min<uint16_t>(y, static_cast<uint16_t>(BoardConfig::DISPLAY_HEIGHT - 1));
}

uint16_t clampPhysicalX(uint16_t x) {
  return std::min<uint16_t>(x, static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_WIDTH - 1));
}

uint16_t clampPhysicalY(uint16_t y) {
  return std::min<uint16_t>(y, static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_HEIGHT - 1));
}

TwoWire &touchWire() { return BoardConfig::TOUCH_USES_WIRE1 ? Wire1 : Wire; }

}  // namespace

bool TouchHandler::begin() {
  lastPollMs_ = 0;
  backoffUntilMs_ = 0;
  lastTouchSampleMs_ = 0;
  consecutiveReadFailures_ = 0;
  emptyTouchSamples_ = 0;
  touchActive_ = false;
  lastX_ = 0;
  lastY_ = 0;

  if (BoardConfig::PIN_TOUCH_RST >= 0) {
    pinMode(BoardConfig::PIN_TOUCH_RST, OUTPUT);
    digitalWrite(BoardConfig::PIN_TOUCH_RST, LOW);
    delay(12);
    digitalWrite(BoardConfig::PIN_TOUCH_RST, HIGH);
    delay(12);
  }

  TwoWire &wire = touchWire();
  wire.beginTransmission(kAddress);
  const uint8_t error = wire.endTransmission();
  initialized_ = (error == 0);

  if (!initialized_) {
    Serial.printf("[touch] Controller not detected at 0x%02X\n", kAddress);
  } else {
    Serial.printf("[touch] Initialized controller=0x%02X\n", kAddress);
  }

  return initialized_;
}

void TouchHandler::end() {
  cancel();
  initialized_ = false;
  if (!BoardConfig::TOUCH_USES_WIRE1) {
    Wire.end();
  }
}

void TouchHandler::cancel() {
  touchActive_ = false;
  lastPollMs_ = 0;
  backoffUntilMs_ = 0;
  lastTouchSampleMs_ = 0;
  consecutiveReadFailures_ = 0;
  emptyTouchSamples_ = 0;
}

void TouchHandler::setUiOrientation(BoardConfig::UiOrientation orientation) {
  if (uiOrientation_ == orientation) {
    return;
  }

  uiOrientation_ = orientation;
  cancel();
}

void TouchHandler::setUiRotated180(bool rotated180) {
  setUiOrientation(rotated180 ? BoardConfig::ROTATED_UI_ORIENTATION
                              : BoardConfig::DEFAULT_UI_ORIENTATION);
}

bool TouchHandler::readTouchPacket(uint8_t *buffer, size_t len) {
  TwoWire &wire = touchWire();

  if (BoardConfig::TOUCH_CONTROLLER == BoardConfig::TouchControllerKind::Ft6336) {
    wire.beginTransmission(kAddress);
    wire.write(kFt6336PointsReg);
    if (wire.endTransmission(false) != 0) {
      return false;
    }

    const size_t readLen = wire.requestFrom(static_cast<uint8_t>(kAddress),
                                            static_cast<size_t>(len), true);
    if (readLen != len) {
      return false;
    }

    for (size_t i = 0; i < len; ++i) {
      buffer[i] = wire.read();
    }
    return true;
  }

  wire.beginTransmission(kAddress);
  wire.write(kReadTouchCommand, sizeof(kReadTouchCommand));
  if (wire.endTransmission(false) != 0) {
    return false;
  }

  const size_t readLen =
      wire.requestFrom(static_cast<uint8_t>(kAddress), static_cast<size_t>(len), true);
  if (readLen != len) {
    return false;
  }

  for (size_t i = 0; i < len; ++i) {
    buffer[i] = wire.read();
  }
  return true;
}

bool TouchHandler::poll(TouchEvent &event) {
  event = TouchEvent{};

  if (!initialized_) {
    return false;
  }

  const uint32_t now = millis();
  if (now < backoffUntilMs_) {
    return false;
  }

  if (now - lastPollMs_ < kPollIntervalMs) {
    return false;
  }
  lastPollMs_ = now;

  const size_t packetLength =
      BoardConfig::TOUCH_CONTROLLER == BoardConfig::TouchControllerKind::Ft6336 ? 5 : 8;
  uint8_t data[8] = {0};
  if (!readTouchPacket(data, packetLength)) {
    backoffUntilMs_ = now + kFailureBackoffMs;
    if (++consecutiveReadFailures_ >= 5) {
      initialized_ = false;
      Serial.println("[touch] Read failed repeatedly, disabling touch polling");
    }
    return false;
  }
  consecutiveReadFailures_ = 0;

  const uint8_t points = BoardConfig::TOUCH_CONTROLLER == BoardConfig::TouchControllerKind::Ft6336
                             ? static_cast<uint8_t>(data[0] & 0x0F)
                             : data[1];
  if (points == 0 || points >= 5) {
    if (touchActive_) {
      ++emptyTouchSamples_;
      if (emptyTouchSamples_ < kReleaseConfirmSamples) {
        return false;
      }

      touchActive_ = false;
      emptyTouchSamples_ = 0;
      event.touched = false;
      event.x = lastX_;
      event.y = lastY_;
      event.phase = TouchPhase::End;
      return true;
    }
    return false;
  }

  backoffUntilMs_ = 0;
  consecutiveReadFailures_ = 0;
  emptyTouchSamples_ = 0;
  lastTouchSampleMs_ = now;

  event.touched = true;
  event.gesture = 0;
  event.phase = touchActive_ ? TouchPhase::Move : TouchPhase::Start;

  uint16_t physicalX = 0;
  uint16_t physicalY = 0;
  if (BoardConfig::TOUCH_CONTROLLER == BoardConfig::TouchControllerKind::Ft6336) {
    physicalX = clampPhysicalX(static_cast<uint16_t>(((data[1] & 0x0F) << 8) | data[2]));
    physicalY = clampPhysicalY(static_cast<uint16_t>(((data[3] & 0x0F) << 8) | data[4]));
  } else {
    const uint16_t rawLongAxis = static_cast<uint16_t>(((data[2] & 0x0F) << 8) | data[3]);
    const uint16_t rawShortAxis = static_cast<uint16_t>(((data[4] & 0x0F) << 8) | data[5]);
    physicalX = clampPhysicalX(rawShortAxis);
    physicalY =
        clampPhysicalY(rawLongAxis >= BoardConfig::PANEL_NATIVE_HEIGHT
                           ? 0
                           : static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_HEIGHT - 1 -
                                                   rawLongAxis));
  }

  switch (uiOrientation_) {
    case BoardConfig::UiOrientation::Portrait:
      event.x = physicalX;
      event.y = physicalY;
      break;
    case BoardConfig::UiOrientation::PortraitFlipped:
      event.x = static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_WIDTH - 1 - physicalX);
      event.y = static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_HEIGHT - 1 - physicalY);
      break;
    case BoardConfig::UiOrientation::Landscape:
      if (BoardConfig::TOUCH_CONTROLLER == BoardConfig::TouchControllerKind::Ft6336) {
        event.x = clampDisplayX(
            static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_HEIGHT - 1 - physicalY));
        event.y = clampDisplayY(physicalX);
      } else {
        event.x =
            clampDisplayX(static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_HEIGHT - 1 - physicalY));
        event.y = clampDisplayY(physicalX);
      }
      break;
    case BoardConfig::UiOrientation::LandscapeFlipped:
    default:
      if (BoardConfig::TOUCH_CONTROLLER == BoardConfig::TouchControllerKind::Ft6336) {
        event.x = clampDisplayX(physicalY);
        event.y =
            clampDisplayY(static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_WIDTH - 1 - physicalX));
      } else {
        event.x = clampDisplayX(physicalY);
        event.y = clampDisplayY(
            static_cast<uint16_t>(BoardConfig::PANEL_NATIVE_WIDTH - 1 - physicalX));
      }
      break;
  }
  touchActive_ = true;
  lastX_ = event.x;
  lastY_ = event.y;

  return true;
}
