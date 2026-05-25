constexpr const char *BOARD_ID = "waveshare_esp32s3_touch_amoled_2_41";
constexpr const char *BOARD_LABEL = "Waveshare ESP32-S3-Touch-AMOLED-2.41";
constexpr DisplayDriverKind DISPLAY_DRIVER = DisplayDriverKind::Rm690b0;
constexpr TouchControllerKind TOUCH_CONTROLLER = TouchControllerKind::Ft6336;
constexpr StorageBusKind STORAGE_BUS = StorageBusKind::SdMmc1Bit;
constexpr PowerManagerKind POWER_MANAGER = PowerManagerKind::DirectGpioBatteryHold;
constexpr bool HAS_LCD_BACKLIGHT = false;
constexpr bool HAS_AUDIO_OUTPUT = false;
constexpr bool TOUCH_USES_WIRE1 = true;

constexpr int PIN_BOOT_BUTTON = 0;
constexpr int PIN_PWR_BUTTON = 15;
constexpr int PIN_BATTERY_ADC = 17;
constexpr int PIN_BATTERY_HOLD = 16;

constexpr int PIN_LCD_CS = 9;
constexpr int PIN_LCD_SCLK = 10;
constexpr int PIN_LCD_DATA0 = 11;
constexpr int PIN_LCD_DATA1 = 12;
constexpr int PIN_LCD_DATA2 = 13;
constexpr int PIN_LCD_DATA3 = 14;
constexpr int PIN_LCD_RST = 21;
constexpr int PIN_LCD_BACKLIGHT = -1;

constexpr int PANEL_NATIVE_WIDTH = 450;
constexpr int PANEL_NATIVE_HEIGHT = 600;
constexpr int DISPLAY_WIDTH = 600;
constexpr int DISPLAY_HEIGHT = 450;
constexpr size_t DISPLAY_TX_CHUNK_BYTES = 48 * 1024;
constexpr bool UI_ROTATED_180 = false;
constexpr UiOrientation DEFAULT_UI_ORIENTATION =
    UI_ROTATED_180 ? UiOrientation::LandscapeFlipped : UiOrientation::Landscape;
constexpr UiOrientation ROTATED_UI_ORIENTATION =
    UI_ROTATED_180 ? UiOrientation::Landscape : UiOrientation::LandscapeFlipped;

// The official demo uses SDMMC by default on GPIO4/5/6 and can also be switched to SDSPI.
constexpr int PIN_SD_CLK = 4;
constexpr int PIN_SD_CMD = 5;
constexpr int PIN_SD_D0 = 6;
constexpr int PIN_I2C_SDA = 47;
constexpr int PIN_I2C_SCL = 48;
constexpr int PIN_TOUCH_SDA = 47;
constexpr int PIN_TOUCH_SCL = 48;
constexpr int PIN_TOUCH_RST = 3;
constexpr uint8_t TOUCH_I2C_ADDRESS = 0x38;

constexpr int TCA9554_ADDRESS = 0x20;
constexpr uint8_t TCA9554_PIN_BATTERY_ADC_ENABLE = 0;
constexpr uint8_t TCA9554_PIN_SYS_EN = 1;
constexpr uint8_t TCA9554_PIN_AUDIO_ENABLE = 0;

constexpr int PIN_AUDIO_MCLK = -1;
constexpr int PIN_AUDIO_BCLK = -1;
constexpr int PIN_AUDIO_WS = -1;
constexpr int PIN_AUDIO_DIN = -1;
constexpr int PIN_AUDIO_DOUT = -1;
constexpr uint8_t ES8311_ADDRESS = 0x00;
