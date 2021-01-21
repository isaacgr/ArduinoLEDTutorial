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
int g_Brightness = 64;
int g_PowerLimit = 900;

// #include <marquee.h>
// #include <twinkle.h>
// #include <comet.h>
// #include <bounce.h>

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000 / x)

// FractionalColor
//
// Returns a fraction of a color; abstracts the fadeToBlack out to this function in case we want to
// improve the color math or do color correction all in one location at a later date

CRGB ColorFraction(CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}

void DrawPixels(float fPos, float count, CRGB color)
{
  // calculate how much the first pixel will hold
  float availFirstPixel = 1.0f - (fPos - (long)(fPos));
  float amtFirstPixel = min(availFirstPixel, count);
  float remaining = min(count, FastLED.size() - fPos);
  int iPos = fPos;

  // Blend (add) in the color of the first partial pixel

  if (remaining > 0.0f)
  {
    FastLED.leds()[iPos++] += ColorFraction(color, amtFirstPixel);
    remaining -= amtFirstPixel;
  }

  // Now draw any full pixels in the middle

  while (remaining > 1.0f)
  {
    FastLED.leds()[iPos++] += color;
    remaining--;
  }

  // Draw tail pixel, up to a single full pixel

  if (remaining > 0.0f)
  {
    FastLED.leds()[iPos] += ColorFraction(color, remaining);
  }
}

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

// void DrawLinesAndGraphicsFrames(int fps)
// {

//   g_OLED.clearBuffer();
//   g_OLED.home();

//   g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());

//   g_OLED.setCursor(3, g_lineHeight * 2 + 2);
//   g_OLED.print("Hello");
//   g_OLED.setCursor(3, g_lineHeight * 3 + 2);
//   g_OLED.print("World");
//   g_OLED.setCursor(3, g_lineHeight * 4 + 2);
//   g_OLED.printf("%03d", fps);

//   for (int x = 0; x < g_OLED.getWidth(); x += 4)
//   {
//     g_OLED.drawLine(x, 0, g_OLED.getWidth() - x, g_OLED.getHeight());
//   }

//   const int reticleY = g_OLED.getHeight() / 2;
//   const int reticleR = g_OLED.getHeight() / 4 - 2;
//   const int reticleX = g_OLED.getWidth() - reticleR - 8;

//   for (int r = reticleR; r > 0; r -= 3)
//   {
//     g_OLED.drawCircle(reticleX, reticleY, r);
//   }
//   g_OLED.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10);
//   g_OLED.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10);

//   g_OLED.sendBuffer();
// }

void loop()
{
  // static bool bLED = LOW;
  // double fps = 0;
  // uint8_t initialHue = 0;
  // const uint8_t deltaHue = 16;
  // const uint8_t hueDensity = 4;
  // BouncingBallEffect balls(NUM_LEDS, 3, 48, true);

  while (true)
  {
    FastLED.clear();
    // bLED = !bLED; // blink the led on and off
    // digitalWrite(LED_BUILTIN, bLED);

    // double dStart = millis() / 1000.0; // display a frame and calculate how long it takes

    // static unsigned long msLastUpdate = millis();
    EVERY_N_MILLISECONDS(20)
    {
      /*

      fadeToBlackBy(g_LEDs, NUM_LEDS, 64);
      int cometSize = 15;
      int iPos = beatsin16(32, 0, NUM_LEDS - cometSize);
      byte hue = beatsin8(60);
      for (int i = iPos; i < iPos + cometSize; i++)
        g_LEDs[iPos] = CHSV(hue, 255, 255);
            */
      DrawMarqueeComparison();
    }

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

    // fill_rainbow(g_LEDs, NUM_LEDS, initialHue += hueDensity, deltaHue);
    // DrawMarquee();
    // DrawTwinkle();
    // DrawComet();
    // balls.Draw();
    FastLED.setBrightness(g_Brightness);
    FastLED.delay(33);

    // double dEnd = millis() / 1000.0;
    // fps = FramesPerSecond(dEnd - dStart);
  }
}
