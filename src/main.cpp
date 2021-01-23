#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define OLED_CLOCK 22
#define OLED_DATA 21
#define OLED_RESET 16

#define NUM_LEDS 40
#define LED_PIN 5
#define COLOR_SEQUENCE GRB

CRGB g_LEDs[NUM_LEDS] = {0};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;
int g_Brightness = 255;
int g_PowerLimit = 2000;

#include "ledgfx.h"
#include "comet.h"
#include "marquee.h"
#include "twinkle.h"
#include "fire.h"

void DrawMarqueeComparison()
{
  static float scroll = 0.0f;
  scroll += 0.1f;
  if (scroll > 5.0)
  {
    scroll -= 5.0;
  }

  for (float i = scroll; i < NUM_LEDS / 2 - 1; i += 5)
  {
    DrawPixels(i, 3, CRGB::Green);
    DrawPixels(NUM_LEDS - 1 - (int)i, 3, CRGB::Red);
  }
}

// FramesPerSecond
//
// Tracks a weighted average to smooth out the values that it calculates as the simple reciprocal
// of the amount of time taken specificed by the caller. So 1/3 of a second is 3fps, and it
// will take up to 10 frames or so to stabalize on that value.

double FramesPerSecond(double seconds)
{
  static double framesPerSecond;
  framesPerSecond = (framesPerSecond * 0.9) + (1.0 / seconds * 0.1);
  return framesPerSecond;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
  {
  }
  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent(); // decent is a negative number, so subtract from the total

  FastLED.addLeds<WS2812B, LED_PIN, COLOR_SEQUENCE>(g_LEDs, NUM_LEDS);
  set_max_power_indicator_LED(LED_BUILTIN);
  FastLED.setMaxPowerInMilliWatts(g_PowerLimit);
}

void loop()
{

  CFireEffect fire(NUM_LEDS, 20, 100, 3, 4, false, false);
  while (true)
  {
    FastLED.clear();
    fire.DrawFire();
    EVERY_N_MILLISECONDS(250)
    {
      g_OLED.clearBuffer();
      g_OLED.setCursor(0, g_lineHeight);
      g_OLED.printf("FPS: %u", FastLED.getFPS());
      g_OLED.setCursor(0, g_lineHeight * 2);
      g_OLED.printf("Power: %u mW", calculate_unscaled_power_mW(g_LEDs, NUM_LEDS));
      g_OLED.setCursor(0, g_lineHeight * 3);
      g_OLED.printf("Brite: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
      g_OLED.sendBuffer();
    }
    FastLED.setBrightness(g_Brightness);
    FastLED.delay(33);
  }
}
