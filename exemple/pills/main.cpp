#include "config.h"
#include "type/colors.h"
#include "timer.h"

LCD lcd;

uint32_t rnd()
{
  static int64_t next = 1;
  next = next * 1103515245 + 12345;
  return next >> 16;
}

#define RADIUS  5
#define COUNT   25
#define X       0
#define Y       1
#define VX      2
#define VY      3

int balls[COUNT][4]; // Q 16.6
RGB colors[] = {
 Aqua,
 Cyan,
 LightCyan,
 PaleTurquoise,
 Aquamarine,
 Turquoise,
 MediumTurquoise,
 DarkTurquoise,
 CadetBlue,
 SteelBlue,
 LightSteelBlue,
 PowderBlue,
 LightBlue,
 SkyBlue,
 LightSkyBlue,
 DeepSkyBlue,
 DodgerBlue,
 CornflowerBlue,
 MediumSlateBlue,
 RoyalBlue,
 Blue,
 MediumBlue,
 DarkBlue,
 Navy,
 MidnightBlue
};

int main(void)
{
  T32_0_PS;
  T32_0_E;
  T32_0_C;

  lcd.init();
  lcd.background(Black);
  lcd.color(Yellow);
  lcd.clear();
  lcd.font(sans_24, 0, 0);

  int count = COUNT;
  while (count--) {
    balls[count][X] = lcd.max_x() << 5;
    balls[count][Y] = lcd.max_y() << 5;
    balls[count][VX] = rnd() & 0x1 ? -(rnd() & 0xff) : rnd() & 0xff;
    balls[count][VY] = rnd() & 0x1 ? -(rnd() & 0xff) : rnd() & 0xff;
  }

loop:
  T32_0_C;
  for (int i = 0; i < COUNT; i++) {
    lcd.color(Black);
    lcd.circle_fill(balls[i][X] >> 6, balls[i][Y] >> 6, RADIUS);

    balls[i][X] += balls[i][VX];
    if (balls[i][X] > ((lcd.max_x() - RADIUS) << 6))  balls[i][VX] = -balls[i][VX];
    if (balls[i][X] < (RADIUS << 6)) balls[i][VX] = -balls[i][VX];

    balls[i][Y] += balls[i][VY];
    if (balls[i][Y] > ((lcd.max_y() - RADIUS) << 6)) balls[i][VY] = -balls[i][VY];
    if (balls[i][Y] < (RADIUS << 6)) balls[i][VY] = -balls[i][VY];

    lcd.color(colors[i]);
    lcd.circle_fill(balls[i][X] >> 6, balls[i][Y] >> 6, RADIUS);
  }

  uint16_t fps = (F_CPU * 10) / T32_0;
  lcd.color(colors[3]);
  lcd.at(10, lcd.max_y() - lcd.get_height());
  lcd.printf(P("FPS: %u.%u"), fps / 10, fps - (fps / 10) * 10);
  goto loop;
}
