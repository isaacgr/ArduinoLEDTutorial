#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define OLED_CLOCK 22
#define OLED_DATA 21
#define OLED_RESET 16

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;

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
  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent(); // decent is a negative number, so subtract from the total
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
  static bool bLED = LOW;
  double fps = 0;

  for (;;)
  {
    bLED = !bLED; // blink the led on and off
    digitalWrite(LED_BUILTIN, bLED);

    double dStart = millis() / 1000.0; // display a frame and calculate how long it takes

    g_OLED.clearBuffer();
    g_OLED.setCursor(0, g_lineHeight);
    g_OLED.printf("FPS: %.1lf", fps);
    g_OLED.sendBuffer();

    double dEnd = millis() / 1000.0;
    fps = FramesPerSecond(dEnd - dStart);
  }
}
