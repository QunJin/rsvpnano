#include "platforms/common/Es8311BoardAudio.h"

#include <Wire.h>

#include "board/BoardAudio.h"
#include "board/BoardConfig.h"
#include "drivers/audio/es8311/Es8311.h"

namespace {

const BoardDrivers::Es8311::Config kEs8311Config = {
    &Wire1,
    Board::Config::ES8311_ADDRESS,
    I2S_NUM_0,
    Board::Config::PIN_AUDIO_MCLK,
    Board::Config::PIN_AUDIO_BCLK,
    Board::Config::PIN_AUDIO_WS,
    Board::Config::PIN_AUDIO_DOUT,
};

}  // namespace

namespace Board::Audio {

bool begin() { return BoardPlatform::Es8311BoardAudio::begin(kEs8311Config); }

bool beep() { return BoardPlatform::Es8311BoardAudio::beep(); }

bool available() { return BoardPlatform::Es8311BoardAudio::available(); }

}  // namespace Board::Audio
