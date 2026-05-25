constexpr const char *BOARD_ID = "waveshare_esp32s3_touch_lcd_3_49";
constexpr const char *BOARD_LABEL = "Waveshare ESP32-S3-Touch-LCD-3.49";
constexpr DisplayDriverKind DISPLAY_DRIVER = DisplayDriverKind::Axs15231b;
constexpr TouchControllerKind TOUCH_CONTROLLER = TouchControllerKind::Axs15231b;
constexpr StorageBusKind STORAGE_BUS = StorageBusKind::SdMmc1Bit;
constexpr PowerManagerKind POWER_MANAGER = PowerManagerKind::Tca9554;
constexpr bool HAS_LCD_BACKLIGHT = true;
constexpr bool HAS_AUDIO_OUTPUT = true;
constexpr bool TOUCH_USES_WIRE1 = false;

constexpr int PIN_BOOT_BUTTON = 0;
constexpr int PIN_PWR_BUTTON = 16;
constexpr int PIN_BATTERY_ADC = 4;
constexpr int PIN_BATTERY_HOLD = -1;

constexpr int PIN_LCD_CS = 9;
constexpr int PIN_LCD_SCLK = 10;
constexpr int PIN_LCD_DATA0 = 11;
constexpr int PIN_LCD_DATA1 = 12;
constexpr int PIN_LCD_DATA2 = 13;
constexpr int PIN_LCD_DATA3 = 14;
constexpr int PIN_LCD_RST = 21;
constexpr int PIN_LCD_BACKLIGHT = 8;

constexpr int PANEL_NATIVE_WIDTH = 172;
constexpr int PANEL_NATIVE_HEIGHT = 640;
constexpr int DISPLAY_WIDTH = 640;
constexpr int DISPLAY_HEIGHT = 172;
constexpr size_t DISPLAY_TX_CHUNK_BYTES = 16 * 1024;
constexpr bool UI_ROTATED_180 = true;  // Keep BOOT/PWR at the top edge in landscape.
constexpr UiOrientation DEFAULT_UI_ORIENTATION =
    UI_ROTATED_180 ? UiOrientation::LandscapeFlipped : UiOrientation::Landscape;
constexpr UiOrientation ROTATED_UI_ORIENTATION =
    UI_ROTATED_180 ? UiOrientation::Landscape : UiOrientation::LandscapeFlipped;

constexpr int PIN_SD_CLK = 41;
constexpr int PIN_SD_CMD = 39;
constexpr int PIN_SD_D0 = 40;
constexpr int PIN_I2C_SDA = 47;
constexpr int PIN_I2C_SCL = 48;
constexpr int PIN_TOUCH_SDA = 17;
constexpr int PIN_TOUCH_SCL = 18;
constexpr int PIN_TOUCH_RST = -1;
constexpr uint8_t TOUCH_I2C_ADDRESS = 0x3B;

constexpr int TCA9554_ADDRESS = 0x20;
constexpr uint8_t TCA9554_PIN_BATTERY_ADC_ENABLE = 1;
constexpr uint8_t TCA9554_PIN_SYS_EN = 6;
constexpr uint8_t TCA9554_PIN_AUDIO_ENABLE = 7;

constexpr int PIN_AUDIO_MCLK = 7;
constexpr int PIN_AUDIO_BCLK = 15;
constexpr int PIN_AUDIO_WS = 46;
constexpr int PIN_AUDIO_DIN = 6;
constexpr int PIN_AUDIO_DOUT = 45;
constexpr uint8_t ES8311_ADDRESS = 0x18;
