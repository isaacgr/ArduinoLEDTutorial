#include <Arduino.h>

void DrawLinesAndGraphicsFrames(int fps)
{

  g_OLED.clearBuffer();
  g_OLED.home();

  g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());

  g_OLED.setCursor(3, g_lineHeight * 2 + 2);
  g_OLED.print("Hello");
  g_OLED.setCursor(3, g_lineHeight * 3 + 2);
  g_OLED.print("World");
  g_OLED.setCursor(3, g_lineHeight * 4 + 2);
  g_OLED.printf("%03d", fps);

  for (int x = 0; x < g_OLED.getWidth(); x += 4)
  {
    g_OLED.drawLine(x, 0, g_OLED.getWidth() - x, g_OLED.getHeight());
  }

  const int reticleY = g_OLED.getHeight() / 2;
  const int reticleR = g_OLED.getHeight() / 4 - 2;
  const int reticleX = g_OLED.getWidth() - reticleR - 8;

  for (int r = reticleR; r > 0; r -= 3)
  {
    g_OLED.drawCircle(reticleX, reticleY, r);
  }
  g_OLED.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10);
  g_OLED.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10);

  g_OLED.sendBuffer();
}