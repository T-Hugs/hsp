#include <M5Unified.h>
#include <WiFi.h>
#include "esp_bt.h"
#include <esp_sleep.h>
#include <esp_system.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <Preferences.h>

// Hog Splitter Pro Logo
#define hs_width 193
#define hs_height 25
static const unsigned char hs_bits[] = {
  0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0xE0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x07, 0x00, 0xE0,
  0xE0, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF,
  0x07, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00,
  0xE0, 0x60, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF,
  0xFF, 0x1F, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xE0, 0x00, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0xFF, 0x3F, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07,
  0x00, 0x00, 0xE0, 0x00, 0xE0, 0x80, 0x03, 0x00, 0x00, 0x00, 0x80, 0xFF,
  0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x0E, 0x70, 0xC0, 0x07, 0xE0, 0x07, 0x80,
  0x07, 0x40, 0x78, 0xE0, 0xE0, 0xF8, 0xEF, 0x3F, 0xF0, 0x41, 0x38, 0xC0,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0E, 0x70, 0xE0, 0x1F, 0xF8, 0x1F,
  0x00, 0x0F, 0xE0, 0xFE, 0xE0, 0xE0, 0xF8, 0xE7, 0x1F, 0xFC, 0xE3, 0x3E,
  0xC0, 0x7F, 0x70, 0xF0, 0xC3, 0xFF, 0x00, 0x0E, 0x70, 0x70, 0x3C, 0x3C,
  0x1E, 0x00, 0x1F, 0xE0, 0xE7, 0xE1, 0xE0, 0xE0, 0x80, 0x03, 0x9E, 0xE3,
  0x3F, 0xE0, 0x3F, 0x00, 0xC0, 0x80, 0xFF, 0x01, 0xFE, 0x7F, 0x38, 0x38,
  0x1C, 0x1C, 0x00, 0xFE, 0xC0, 0xC3, 0xE1, 0xE0, 0xE0, 0x80, 0x03, 0x07,
  0xC7, 0x07, 0xE0, 0x3F, 0x00, 0x40, 0x00, 0xFF, 0x01, 0xFE, 0x7F, 0x38,
  0x38, 0x1E, 0x1C, 0x00, 0xFC, 0xC1, 0xC3, 0xE3, 0xE0, 0xE0, 0x80, 0x03,
  0x07, 0xC7, 0x03, 0xE0, 0x3F, 0x04, 0x04, 0x1C, 0xFF, 0x01, 0x1E, 0x70,
  0x38, 0x78, 0x0E, 0x1C, 0x00, 0xF0, 0xC3, 0x81, 0xE3, 0xE0, 0xE0, 0x80,
  0x03, 0x87, 0xC3, 0x01, 0xE0, 0x3F, 0x04, 0x00, 0x1C, 0xFE, 0x01, 0x0E,
  0x70, 0x3C, 0x78, 0x0E, 0x1C, 0x00, 0xC0, 0xC7, 0x81, 0xE3, 0xE0, 0xE0,
  0x80, 0x03, 0xC7, 0xC3, 0x01, 0xE0, 0x3F, 0x00, 0x00, 0x1C, 0xFE, 0x01,
  0x0E, 0x70, 0x3C, 0x78, 0x0E, 0x1C, 0x00, 0x80, 0xC7, 0x81, 0xE3, 0xE0,
  0xE0, 0x80, 0x03, 0xFF, 0xC1, 0x01, 0xE0, 0x3F, 0x20, 0x20, 0x1C, 0xFE,
  0x01, 0x0E, 0x70, 0x3C, 0x78, 0x0E, 0x1E, 0x00, 0x00, 0xC7, 0xC1, 0xE3,
  0xE0, 0xE0, 0x80, 0x03, 0x3F, 0xC0, 0x01, 0xE0, 0x3F, 0x38, 0x00, 0x1C,
  0xFF, 0x01, 0x0E, 0x70, 0x38, 0x38, 0x0E, 0x1E, 0x00, 0x00, 0xC7, 0xC1,
  0xE1, 0xE0, 0xE0, 0x80, 0x03, 0x07, 0xC0, 0x01, 0xE0, 0x3F, 0x3C, 0x00,
  0x1C, 0xFF, 0x01, 0x0E, 0x70, 0x38, 0x38, 0x1E, 0x1E, 0x00, 0x81, 0xC7,
  0xC1, 0xE1, 0xE0, 0xE0, 0x80, 0x03, 0x07, 0xC0, 0x01, 0xC0, 0x3F, 0x3C,
  0x04, 0x00, 0xFF, 0x00, 0x0E, 0x70, 0x78, 0x3C, 0x3C, 0x1F, 0x80, 0xFF,
  0xC3, 0xE1, 0xE0, 0xE0, 0xE0, 0x81, 0x07, 0x0F, 0xC3, 0x01, 0xC0, 0x3F,
  0x3C, 0x8C, 0x80, 0xFF, 0x00, 0x0E, 0x70, 0xF0, 0x1F, 0xF8, 0x19, 0x80,
  0xFF, 0xC1, 0x7F, 0xE0, 0xE3, 0xC1, 0x0F, 0x3F, 0xFE, 0xC3, 0x01, 0x80,
  0x7F, 0x7E, 0x9E, 0xE3, 0x7F, 0x00, 0x0E, 0x70, 0xE0, 0x07, 0xF0, 0x38,
  0x00, 0x7C, 0xC0, 0x3F, 0xC0, 0xC3, 0x81, 0x0F, 0x3E, 0xF8, 0xC0, 0x01,
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x38, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1C, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, };

struct HistoryRow;

// ---------- Preferences ----------
static Preferences prefs;

// ---------- Pin definitions (CoreInk — M5Unified mapping) ----------
// BtnEXT           → GPIO 5  (Physical button on top — split / exit settings)
// BtnA / BtnB / BtnC → GPIO 39 / 38 / 37 (front: up / middle / down)
// BtnPWR           → GPIO 27 (Physical side POWER switch — use M5.BtnPWR in code!)
// GPIO 12 holds the power latch HIGH; must remain driven across sleep.
static constexpr gpio_num_t PIN_BTN_EXT = GPIO_NUM_5;
static constexpr gpio_num_t PIN_BTN_PWR = GPIO_NUM_27;
static constexpr gpio_num_t PIN_BTN_A = GPIO_NUM_39;
static constexpr gpio_num_t PIN_BTN_B = GPIO_NUM_38;
static constexpr gpio_num_t PIN_BTN_C = GPIO_NUM_37;
static constexpr gpio_num_t PIN_PWR_LATCH = GPIO_NUM_12;

// ---------- Auto shutoff ----------
enum class AutoShutoff {
  Disabled = 0,
  Minutes1 = 1,
  Minutes10 = 10,
  Minutes15 = 15,
  Minutes20 = 20,
  Minutes30 = 30,
};

static AutoShutoff autoShutoff = AutoShutoff::Disabled;

static constexpr uint32_t MAIN_DISPLAY_IDLE_CLEAR_MS = 60 * 1000;

// Settings UI: press BtnB (middle) on the main screen to open; BtnEXT saves and exits.
static bool settingsConfiguring = false;
static bool settingsAwaitBtnBRelease = false;

enum class SettingsPage : uint8_t {
  AutoShutoff = 0,
  ToneVolume,
  TonePitch,
  ToneDuration,
  FontSize,
  Count,
};
static SettingsPage settingsPage = SettingsPage::AutoShutoff;

// Buzzer: M5.Speaker.setVolume(0–255) + tone(hz, ms) — see M5Unified Speaker class docs.
static constexpr uint8_t TONE_LEVEL_COUNT = 5;
static const uint8_t kToneVolumeLevels[TONE_LEVEL_COUNT] = {0, 75, 128, 190, 245};
static uint8_t toneVolumeIndex = 2;

static const float kTonePitchHz[TONE_LEVEL_COUNT] = {2600.f, 3300.f, 4000.f, 4700.f, 5400.f};
static uint8_t tonePitchIndex = 2;

static const uint32_t kToneDurationMs[TONE_LEVEL_COUNT] = {35, 48, 62, 80, 105};
static uint8_t toneDurationIndex = 2;

enum class FontSizePreset : uint8_t { Small = 0, Medium = 1, Large = 2 };
static FontSizePreset fontSizePreset = FontSizePreset::Medium;

static const char* autoShutoffLabel(AutoShutoff as) {
  switch (as) {
    case AutoShutoff::Disabled: return "Disabled";
    case AutoShutoff::Minutes1: return "1 minute";
    case AutoShutoff::Minutes10: return "10 minutes";
    case AutoShutoff::Minutes15: return "15 minutes";
    case AutoShutoff::Minutes20: return "20 minutes";
    case AutoShutoff::Minutes30: return "30 minutes";
    default: return "Unknown";
  }
}

static AutoShutoff nextAutoShutoff(AutoShutoff current) {
  switch (current) {
    case AutoShutoff::Disabled: return AutoShutoff::Minutes1;
    case AutoShutoff::Minutes1: return AutoShutoff::Minutes10;
    case AutoShutoff::Minutes10: return AutoShutoff::Minutes15;
    case AutoShutoff::Minutes15: return AutoShutoff::Minutes20;
    case AutoShutoff::Minutes20: return AutoShutoff::Minutes30;
    case AutoShutoff::Minutes30: return AutoShutoff::Disabled;
    default: return AutoShutoff::Disabled;
  }
}

static AutoShutoff prevAutoShutoff(AutoShutoff current) {
  switch (current) {
    case AutoShutoff::Disabled: return AutoShutoff::Minutes30;
    case AutoShutoff::Minutes1: return AutoShutoff::Disabled;
    case AutoShutoff::Minutes10: return AutoShutoff::Minutes1;
    case AutoShutoff::Minutes15: return AutoShutoff::Minutes10;
    case AutoShutoff::Minutes20: return AutoShutoff::Minutes15;
    case AutoShutoff::Minutes30: return AutoShutoff::Minutes20;
    default: return AutoShutoff::Disabled;
  }
}

static void loadAllSettings() {
  prefs.begin("curltimer", true);
  int val = prefs.getInt("autoshutoff", 0);
  switch (val) {
    case 1: autoShutoff = AutoShutoff::Minutes1; break;
    case 10: autoShutoff = AutoShutoff::Minutes10; break;
    case 15: autoShutoff = AutoShutoff::Minutes15; break;
    case 20: autoShutoff = AutoShutoff::Minutes20; break;
    case 30: autoShutoff = AutoShutoff::Minutes30; break;
    default: autoShutoff = AutoShutoff::Disabled; break;
  }

  toneVolumeIndex = prefs.getUChar("tonevol", 2);
  tonePitchIndex = prefs.getUChar("tonepitch", 2);
  toneDurationIndex = prefs.getUChar("tonedur", 2);
  uint8_t fs = prefs.getUChar("fontsize", 1);

  prefs.end();

  if (toneVolumeIndex >= TONE_LEVEL_COUNT) toneVolumeIndex = 2;
  if (tonePitchIndex >= TONE_LEVEL_COUNT) tonePitchIndex = 2;
  if (toneDurationIndex >= TONE_LEVEL_COUNT) toneDurationIndex = 2;
  if (fs > 2) fs = 1;
  fontSizePreset = static_cast<FontSizePreset>(fs);
}

static void saveAllSettings() {
  int asval = 0;
  switch (autoShutoff) {
    case AutoShutoff::Minutes1: asval = 1; break;
    case AutoShutoff::Minutes10: asval = 10; break;
    case AutoShutoff::Minutes15: asval = 15; break;
    case AutoShutoff::Minutes20: asval = 20; break;
    case AutoShutoff::Minutes30: asval = 30; break;
    default: asval = 0; break;
  }

  prefs.begin("curltimer", false);
  prefs.putInt("autoshutoff", asval);
  prefs.putUChar("tonevol", toneVolumeIndex);
  prefs.putUChar("tonepitch", tonePitchIndex);
  prefs.putUChar("tonedur", toneDurationIndex);
  prefs.putUChar("fontsize", static_cast<uint8_t>(fontSizePreset));
  prefs.end();
}

static void applySpeakerVolumeFromConfig() { M5.Speaker.setVolume(kToneVolumeLevels[toneVolumeIndex]); }

static void playToneIfAudible(float hz, uint32_t ms) {
  applySpeakerVolumeFromConfig();
  if (kToneVolumeLevels[toneVolumeIndex] == 0) return;
  M5.Speaker.tone(hz, ms);
}

static float currentToneHz() { return kTonePitchHz[tonePitchIndex]; }

static uint32_t currentToneMs() { return kToneDurationMs[toneDurationIndex]; }

static int historyVisibleRows() {
  switch (fontSizePreset) {
    case FontSizePreset::Small: return 9;
    case FontSizePreset::Medium: return 6;
    case FontSizePreset::Large: return 3;
  }
  return 6;
}

static int historyRowHeightPx() {
  switch (fontSizePreset) {
    case FontSizePreset::Small: return 15;
    case FontSizePreset::Medium: return 20;
    case FontSizePreset::Large: return 34;
  }
  return 20;
}

static FontSizePreset nextFontSize(FontSizePreset p) {
  switch (p) {
    case FontSizePreset::Small: return FontSizePreset::Medium;
    case FontSizePreset::Medium: return FontSizePreset::Large;
    case FontSizePreset::Large: return FontSizePreset::Small;
  }
  return FontSizePreset::Medium;
}

static FontSizePreset prevFontSize(FontSizePreset p) {
  switch (p) {
    case FontSizePreset::Small: return FontSizePreset::Large;
    case FontSizePreset::Medium: return FontSizePreset::Small;
    case FontSizePreset::Large: return FontSizePreset::Medium;
  }
  return FontSizePreset::Medium;
}

static const char* fontSizeLabel(FontSizePreset p) {
  switch (p) {
    case FontSizePreset::Small: return "Small (9 rows)";
    case FontSizePreset::Medium: return "Medium (6 rows)";
    case FontSizePreset::Large: return "Large (3 rows)";
  }
  return "?";
}

static SettingsPage nextSettingsPage(SettingsPage p) {
  uint8_t n = static_cast<uint8_t>(p) + 1;
  if (n >= static_cast<uint8_t>(SettingsPage::Count)) {
    return SettingsPage::AutoShutoff;
  }
  return static_cast<SettingsPage>(n);
}

static void playConfigPreviewTone() { playToneIfAudible(currentToneHz(), currentToneMs()); }

static void fillSettingsValueString(char* buf, size_t bufLen) {
  switch (settingsPage) {
    case SettingsPage::AutoShutoff:
      snprintf(buf, bufLen, "%s", autoShutoffLabel(autoShutoff));
      break;
    case SettingsPage::ToneVolume:
      if (toneVolumeIndex == 0) {
        snprintf(buf, bufLen, "Off");
      } else {
        snprintf(buf, bufLen, "Level %u  (%u)", static_cast<unsigned>(toneVolumeIndex),
                 static_cast<unsigned>(kToneVolumeLevels[toneVolumeIndex]));
      }
      break;
    case SettingsPage::TonePitch:
      snprintf(buf, bufLen, "%.0f Hz", static_cast<double>(kTonePitchHz[tonePitchIndex]));
      break;
    case SettingsPage::ToneDuration:
      snprintf(buf, bufLen, "%lu ms", static_cast<unsigned long>(kToneDurationMs[toneDurationIndex]));
      break;
    case SettingsPage::FontSize:
      snprintf(buf, bufLen, "%s", fontSizeLabel(fontSizePreset));
      break;
    default:
      snprintf(buf, bufLen, "?");
      break;
  }
}

static const char* settingsPageTitle(SettingsPage p) {
  switch (p) {
    case SettingsPage::AutoShutoff: return "Auto-shutoff";
    case SettingsPage::ToneVolume: return "Tone volume";
    case SettingsPage::TonePitch: return "Tone pitch";
    case SettingsPage::ToneDuration: return "Tone duration";
    case SettingsPage::FontSize: return "Font size";
    default: return "Settings";
  }
}

// ---------- History ----------
struct HistoryRow {
  float left;   // -1 = empty
  float right;  // -1 = empty
};

static constexpr int HISTORY_CAPACITY = 10;
static HistoryRow history[HISTORY_CAPACITY];
static int historyCount = 0;

// ---------- Timing state ----------
static uint32_t lastPressMs = 0;
static bool haveLastPress = false;
static float lastElapsed = -1.0f;
static bool lastTooLong = false;
// BtnPWR: only treat M5.BtnPWR.wasPressed() as power-off intent. A raw press-edge
// (wasPressed miss after light sleep) caused false power-down when waking with BtnEXT.
static uint32_t lastSplitTapForIdleClearMs = 0;
// esp_timer_get_time() at last BtnEXT split — survives uses where millis() stalls in light sleep.
static uint64_t lastSplitTapEspTimerUs = 0;

enum class LightSleepTimerRole : uint8_t {
  None = 0,
  IdleClearMain,
  AutoShutoff,
};
static LightSleepTimerRole lastLightSleepTimerRole = LightSleepTimerRole::None;
static bool pendingIdleClearAfterLightSleep = false;
static bool pendingAutoShutoffAfterLightSleep = false;

// ---------- Auto-shutoff tracking ----------
static uint32_t lastActivityMs = 0;
static bool autoShutoffTriggered = false;

// ---------- History helpers ----------
static void clearHistory() {
  for (int i = 0; i < HISTORY_CAPACITY; i++) {
    history[i].left = -1.0f;
    history[i].right = -1.0f;
  }
  historyCount = 0;
}

static void pushRow(const HistoryRow& row) {
  if (historyCount < HISTORY_CAPACITY) {
    history[historyCount++] = row;
  } else {
    for (int i = 0; i < HISTORY_CAPACITY - 1; i++) {
      history[i] = history[i + 1];
    }
    history[HISTORY_CAPACITY - 1] = row;
  }
}

static void addToHistory(float seconds) {
  if (seconds > 20.0f) {
    return;
  }

  if (seconds < 5.5f) {
    HistoryRow r{seconds, -1.0f};
    pushRow(r);
  } else {
    if (historyCount > 0 && history[historyCount - 1].left >= 0.0f &&
        history[historyCount - 1].right < 0.0f) {
      history[historyCount - 1].right = seconds;
    } else {
      HistoryRow r{-1.0f, seconds};
      pushRow(r);
    }
  }
}

// ---------- Drawing ----------
static void drawBatteryIndicator(int rightEdgeX, int centerY) {
  auto& d = M5.Display;
  const int bw = 36;
  const int bh = 14;

  int pct = M5.Power.getBatteryLevel();

  char pctBuf[8];
  snprintf(pctBuf, sizeof(pctBuf), "%d%%", pct);

  d.setFont(&fonts::Font0);
  d.setTextSize(1);
  d.setTextColor(TFT_BLACK, TFT_WHITE);

  int iconX = rightEdgeX - bw - 2;
  int iconY = centerY - bh / 2;

  d.drawRect(iconX, iconY, bw, bh, TFT_BLACK);
  d.fillRect(iconX + bw, iconY + 4, 2, bh - 8, TFT_BLACK);

  int fillW = (bw - 4) * pct / 100;
  if (fillW > 0) {
    d.fillRect(iconX + 2, iconY + 2, fillW, bh - 4, TFT_BLACK);
  }

  d.setTextDatum(middle_right);
  d.drawString(pctBuf, iconX - 4, centerY);
}

static void drawDisplay() {
  auto& d = M5.Display;
  const int w = d.width();
  const int h = d.height();

  d.startWrite();
  d.fillScreen(TFT_WHITE);
  d.setTextColor(TFT_BLACK, TFT_WHITE);

  // ----- Big elapsed time (top) -----
  d.setFont(&fonts::Font7);
  d.setTextSize(1);
  d.setTextDatum(middle_center);

  char buf[16];
  if (lastElapsed < 0.0f || lastTooLong) {
    strcpy(buf, "--.--");
  } else {
    snprintf(buf, sizeof(buf), "%5.2f", lastElapsed);
  }
  d.drawString(buf, w / 2, 32);

  // ----- History -----
  switch (fontSizePreset) {
    case FontSizePreset::Small: d.setFont(&fonts::FreeSansBold9pt7b); break;
    case FontSizePreset::Medium: d.setFont(&fonts::FreeSansBold12pt7b); break;
    case FontSizePreset::Large: d.setFont(&fonts::FreeSansBold18pt7b); break;
  }
  d.setTextSize(1);
  d.setTextDatum(middle_left);

  const int historyTop = (fontSizePreset == FontSizePreset::Small) ? 68 : 70;
  const int rowH = historyRowHeightPx();
  const int leftX = 6;
  const int rightX = (fontSizePreset == FontSizePreset::Small) ? (w / 2 - 8) : (w / 2 + 4);
  const int visibleRows = historyVisibleRows();
  const int shown = (historyCount < visibleRows) ? historyCount : visibleRows;

  for (int i = 0; i < shown; i++) {
    int rowIndex = historyCount - 1 - i;
    int y = historyTop + i * rowH + rowH / 2;
    char b[16];

    if (history[rowIndex].left >= 0.0f) {
      snprintf(b, sizeof(b), "%.2f", history[rowIndex].left);
      d.drawString(b, leftX, y);
    }
    if (history[rowIndex].right >= 0.0f) {
      snprintf(b, sizeof(b), "%.2f", history[rowIndex].right);
      d.drawString(b, rightX, y);
    }
  }

  // ----- Battery (bottom-right) -----
  const int battCenterY = h - 12;
  const int battRightX = w - 4;
  drawBatteryIndicator(battRightX, battCenterY);

  d.endWrite();
  d.display();
}

static void drawHSPLogo() {
  auto& d = M5.Display;
  const int w = d.width();
  const int x = (w - hs_width) / 2;
  const int y = 2;
  d.drawXBitmap(x, y, hs_bits, hs_width, hs_height, TFT_BLACK);
}

static void drawPowerOffScreen() {
  auto& d = M5.Display;
  const int w = d.width();
  const int h = d.height();

  d.startWrite();
  d.fillScreen(TFT_WHITE);
  d.setTextColor(TFT_BLACK, TFT_WHITE);

  d.setFont(&fonts::FreeSansBold18pt7b);
  d.setTextSize(1);
  d.setTextDatum(middle_center);
  d.drawString("Power Off", w / 2, h / 2 - 24);

  d.setFont(&fonts::FreeSans9pt7b);
  d.drawString("Press PWR to wake", w / 2, h / 2 + 8);

  d.drawFastHLine(20, h / 2 - 4, w - 40, TFT_BLACK);

  drawBatteryIndicator(w - 4, h - 12);
  drawHSPLogo();

  d.endWrite();
  d.display();
}

static void drawSettingsConfig() {
  auto& d = M5.Display;
  const int w = d.width();
  const int h = d.height();

  char valueBuf[48];
  fillSettingsValueString(valueBuf, sizeof(valueBuf));

  d.startWrite();
  d.fillScreen(TFT_WHITE);
  d.setTextColor(TFT_BLACK, TFT_WHITE);

  d.setFont(&fonts::FreeSansBold12pt7b);
  d.setTextSize(1);
  d.setTextDatum(middle_center);
  d.drawString(settingsPageTitle(settingsPage), w / 2, h / 2 - 28);

  d.drawString(valueBuf, w / 2, h / 2 + 6);

  d.drawFastHLine(20, h / 2 - 8, w - 40, TFT_BLACK);

  d.endWrite();
  d.display();
}

// ---------- Power ----------
static void disableRadios() {
  WiFi.mode(WIFI_OFF);
  WiFi.disconnect(true);
  btStop();
  esp_bt_controller_disable();
}

// Forward decls (so powerDown can use them).
static void beepPowerOff();
static void waitForPeripherals();

static void powerDown() {
  beepPowerOff();

  drawPowerOffScreen();

  waitForPeripherals();
  M5.Display.sleep();

  // Release latch holds we may have set for light sleep so we can
  // actively drive GPIO 12 low and cut battery power.
  gpio_hold_dis(PIN_PWR_LATCH);

  pinMode(PIN_PWR_LATCH, OUTPUT);
  digitalWrite(PIN_PWR_LATCH, LOW);

  // USB-powered fallback: latch low may still leave the ESP running; light-sleep GPIO
  // wake on BtnPWR was unreliable here after latch release. Poll PWR (active-low) and
  // cold-start firmware — same outcome as the panel reset button (setup() reasserts latch).
  gpio_hold_dis(PIN_BTN_PWR);

  constexpr uint32_t kPwrRestartHoldMs = 120;
  uint32_t pwrLowSinceMs = 0;
  while (true) {
    M5.update();
    bool pwrPressed = gpio_get_level(PIN_BTN_PWR) == 0;
    uint32_t t = millis();
    if (pwrPressed) {
      if (pwrLowSinceMs == 0) {
        pwrLowSinceMs = t;
      } else if ((uint32_t)(t - pwrLowSinceMs) >= kPwrRestartHoldMs) {
        esp_restart();
      }
    } else {
      pwrLowSinceMs = 0;
    }
    delay(10);
  }
}

// ---------- Beeps ----------
static void beepSplit() { playToneIfAudible(currentToneHz(), currentToneMs()); }

static void beepPowerOff() {
  playToneIfAudible(currentToneHz() * 0.55f, currentToneMs() * 2);
}

static void beepConfigStep() {
  playConfigPreviewTone();
}

static void waitForPeripherals() {
  while (M5.Speaker.isPlaying()) {
    delay(5);
  }
  M5.Display.waitDisplay();
}

// ---------- Light sleep ----------
//
// Idle policy: light sleep until BtnEXT/BtnPWR or (optional) RTC timer for idle-clear /
// auto-shutoff — millis() alone does not advance reliably across light sleep.
static void enterLightSleepUntilEvent() {
  // Never sleep with a button held — we'd miss the release edge and
  // break long-press detection.
  if (M5.BtnEXT.isPressed() || M5.BtnPWR.isPressed() || M5.BtnA.isPressed() || M5.BtnB.isPressed() ||
      M5.BtnC.isPressed()) {
    return;
  }

  // Never sleep in the settings UI.
  if (settingsConfiguring) return;

  // Don't sleep if a tone is still playing.
  if (M5.Speaker.isPlaying()) return;

  // Make sure pending EPD work is done before we suspend.
  M5.Display.waitDisplay();

  // Hold the power latch HIGH across sleep so the device doesn't
  // power off when the CPU pauses driving GPIOs.
  gpio_hold_en(PIN_PWR_LATCH);

  // Wake on any user button going low (active-low).
  gpio_wakeup_enable(PIN_BTN_EXT, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(PIN_BTN_PWR, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(PIN_BTN_A, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(PIN_BTN_B, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(PIN_BTN_C, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();

  constexpr uint64_t kNoTimerUs = UINT64_MAX;

  uint64_t shutoffUs = kNoTimerUs;
  if (autoShutoff != AutoShutoff::Disabled) {
    uint32_t timeoutMs = static_cast<uint32_t>(autoShutoff) * 60 * 1000;
    uint32_t elapsed = millis() - lastActivityMs;
    if (timeoutMs > elapsed) {
      shutoffUs = static_cast<uint64_t>(timeoutMs - elapsed) * 1000ULL;
    } else {
      shutoffUs = 1000ULL;
    }
  }

  uint64_t idleUs = kNoTimerUs;
  if (lastElapsed >= 0.0f && !lastTooLong && lastSplitTapEspTimerUs != 0) {
    uint64_t elapsedTapUs = esp_timer_get_time() - lastSplitTapEspTimerUs;
    uint64_t limitUs = static_cast<uint64_t>(MAIN_DISPLAY_IDLE_CLEAR_MS) * 1000ULL;
    if (elapsedTapUs >= limitUs) {
      idleUs = 1000ULL;
    } else {
      idleUs = limitUs - elapsedTapUs;
    }
  }

  lastLightSleepTimerRole = LightSleepTimerRole::None;
  uint64_t wakeUs = kNoTimerUs;
  if (idleUs < shutoffUs) {
    wakeUs = idleUs;
    lastLightSleepTimerRole = LightSleepTimerRole::IdleClearMain;
  } else if (shutoffUs < idleUs) {
    wakeUs = shutoffUs;
    lastLightSleepTimerRole = LightSleepTimerRole::AutoShutoff;
  } else if (idleUs != kNoTimerUs && idleUs == shutoffUs) {
    wakeUs = idleUs;
    lastLightSleepTimerRole = LightSleepTimerRole::AutoShutoff;
  }

  if (wakeUs != kNoTimerUs) {
    esp_sleep_enable_timer_wakeup(wakeUs);
  }

  esp_light_sleep_start();

  esp_sleep_wakeup_cause_t wc = esp_sleep_get_wakeup_cause();
  if (wc == ESP_SLEEP_WAKEUP_TIMER) {
    switch (lastLightSleepTimerRole) {
      case LightSleepTimerRole::IdleClearMain:
        pendingIdleClearAfterLightSleep = true;
        break;
      case LightSleepTimerRole::AutoShutoff:
        pendingAutoShutoffAfterLightSleep = true;
        break;
      default:
        break;
    }
  }
  lastLightSleepTimerRole = LightSleepTimerRole::None;

  // Resume: release the latch hold so normal code can drive GPIO 12
  // again (e.g. powerDown() needs to pull it low).
  gpio_hold_dis(PIN_PWR_LATCH);
}

static void applyIdleMainClearDisplay() {
  lastElapsed = -1.0f;
  lastTooLong = false;
  haveLastPress = false;
  lastSplitTapForIdleClearMs = 0;
  lastSplitTapEspTimerUs = 0;
  drawDisplay();
}

// Clear main elapsed if no BtnEXT-split activity for MAIN_DISPLAY_IDLE_CLEAR_MS.
// Runs at the start of every loop() iteration so early returns cannot skip it.
static void maybeClearIdleMainElapsed(uint32_t nowMs) {
  if (settingsConfiguring) return;
  if (lastElapsed < 0.0f || lastTooLong) return;

  bool dueMillis = lastSplitTapForIdleClearMs != 0 &&
                   (uint32_t)(nowMs - lastSplitTapForIdleClearMs) >= MAIN_DISPLAY_IDLE_CLEAR_MS;
  bool dueEspTimer =
      lastSplitTapEspTimerUs != 0 &&
      (esp_timer_get_time() - lastSplitTapEspTimerUs >=
       static_cast<uint64_t>(MAIN_DISPLAY_IDLE_CLEAR_MS) * 1000ULL);

  if (!dueMillis && !dueEspTimer) return;
  applyIdleMainClearDisplay();
}

// ---------- Event handlers ----------
static void handleSplit() {
  uint32_t now = millis();
  lastSplitTapForIdleClearMs = now;
  lastSplitTapEspTimerUs = esp_timer_get_time();
  beepSplit();

  if (haveLastPress) {
    uint32_t deltaMs = now - lastPressMs;
    float seconds = deltaMs / 1000.0f;

    if (seconds > 59.0f) {
      lastTooLong = true;
    } else {
      lastElapsed = seconds;
      lastTooLong = false;
      addToHistory(seconds);
    }
  }

  lastPressMs = now;
  haveLastPress = true;
  lastActivityMs = now;
  drawDisplay();
}

// ---------- Arduino entry ----------
void setup() {
  // CoreInk power latch — must be asserted IMMEDIATELY on boot,
  // otherwise a brief PWR-button press won't keep the device on.
  pinMode(PIN_PWR_LATCH, OUTPUT);
  digitalWrite(PIN_PWR_LATCH, HIGH);

  // If we previously slept with the latch held, release the hold so
  // the digitalWrite above is actually authoritative.
  gpio_hold_dis(PIN_PWR_LATCH);

  auto cfg = M5.config();
  M5.begin(cfg);

  disableRadios();
  setCpuFrequencyMhz(80);

  // Force display controller reset and reinit
  M5.Display.display();
  delay(100);
  M5.Display.init();
  M5.Display.setEpdMode(epd_mode_t::epd_fastest);

  loadAllSettings();
  applySpeakerVolumeFromConfig();

  clearHistory();
  lastActivityMs = millis();
  drawDisplay();
}

void loop() {
  M5.update();

  if (pendingIdleClearAfterLightSleep) {
    pendingIdleClearAfterLightSleep = false;
    if (!settingsConfiguring && lastElapsed >= 0.0f && !lastTooLong) {
      applyIdleMainClearDisplay();
    }
  }
  if (pendingAutoShutoffAfterLightSleep) {
    pendingAutoShutoffAfterLightSleep = false;
    autoShutoffTriggered = true;
    powerDown();
  }

  uint32_t now = millis();
  maybeClearIdleMainElapsed(now);

  bool btnPWRWantsPowerOff = M5.BtnPWR.wasPressed();
  if (btnPWRWantsPowerOff) {
    powerDown();
  }

  if (settingsConfiguring) {
    if (settingsAwaitBtnBRelease && !M5.BtnB.isPressed()) {
      settingsAwaitBtnBRelease = false;
    }

    if (M5.BtnEXT.wasPressed()) {
      saveAllSettings();
      applySpeakerVolumeFromConfig();
      settingsConfiguring = false;
      settingsAwaitBtnBRelease = false;
      drawDisplay();
      delay(20);
      return;
    }

    const bool bOk = !settingsAwaitBtnBRelease;

    if (M5.BtnB.wasPressed() && bOk) {
      settingsPage = nextSettingsPage(settingsPage);
      drawSettingsConfig();
    } else if (M5.BtnA.wasPressed()) {
      switch (settingsPage) {
        case SettingsPage::AutoShutoff:
          autoShutoff = nextAutoShutoff(autoShutoff);
          beepConfigStep();
          break;
        case SettingsPage::ToneVolume:
          toneVolumeIndex = (toneVolumeIndex + 1) % TONE_LEVEL_COUNT;
          playConfigPreviewTone();
          break;
        case SettingsPage::TonePitch:
          tonePitchIndex = (tonePitchIndex + 1) % TONE_LEVEL_COUNT;
          playConfigPreviewTone();
          break;
        case SettingsPage::ToneDuration:
          toneDurationIndex = (toneDurationIndex + 1) % TONE_LEVEL_COUNT;
          playConfigPreviewTone();
          break;
        case SettingsPage::FontSize:
          fontSizePreset = nextFontSize(fontSizePreset);
          beepConfigStep();
          break;
        default: break;
      }
      drawSettingsConfig();
    } else if (M5.BtnC.wasPressed()) {
      switch (settingsPage) {
        case SettingsPage::AutoShutoff:
          autoShutoff = prevAutoShutoff(autoShutoff);
          beepConfigStep();
          break;
        case SettingsPage::ToneVolume:
          toneVolumeIndex = (toneVolumeIndex + TONE_LEVEL_COUNT - 1) % TONE_LEVEL_COUNT;
          playConfigPreviewTone();
          break;
        case SettingsPage::TonePitch:
          tonePitchIndex = (tonePitchIndex + TONE_LEVEL_COUNT - 1) % TONE_LEVEL_COUNT;
          playConfigPreviewTone();
          break;
        case SettingsPage::ToneDuration:
          toneDurationIndex = (toneDurationIndex + TONE_LEVEL_COUNT - 1) % TONE_LEVEL_COUNT;
          playConfigPreviewTone();
          break;
        case SettingsPage::FontSize:
          fontSizePreset = prevFontSize(fontSizePreset);
          beepConfigStep();
          break;
        default: break;
      }
      drawSettingsConfig();
    }

    delay(20);
    return;
  }

  if (M5.BtnB.wasPressed()) {
    settingsConfiguring = true;
    settingsPage = SettingsPage::AutoShutoff;
    settingsAwaitBtnBRelease = true;
    beepConfigStep();
    drawSettingsConfig();
    delay(20);
    return;
  }

  bool extWasPressed = M5.BtnEXT.wasPressed();
  if (extWasPressed) {
    handleSplit();
  }

  // Check for auto-shutoff timeout
  if (autoShutoff != AutoShutoff::Disabled && !autoShutoffTriggered) {
    uint32_t timeoutMs = static_cast<uint32_t>(autoShutoff) * 60 * 1000;
    if (now - lastActivityMs >= timeoutMs) {
      autoShutoffTriggered = true;
      powerDown();
    }
  }

  // Idle: try to enter light sleep instead of busy-waiting.
  if (!settingsConfiguring && !M5.BtnEXT.isPressed() && !M5.BtnPWR.isPressed() && !M5.BtnA.isPressed() &&
      !M5.BtnB.isPressed() && !M5.BtnC.isPressed()) {
    enterLightSleepUntilEvent();
    return;
  }

  delay(20);
}