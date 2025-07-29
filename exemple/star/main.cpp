#include "lcd.h"

#define COUNT_STAR  40
LCD lcd;

uint32_t rnd()
{
  static int64_t next = 1;
  next = next * 1103515245 + 12345;
  return next >> 16;
}

int main(void)
{
  int16_t cycle[COUNT_STAR][2];
  // RGB black(0, 16, 32);
  // RGB white(255, 255, 96);

  lcd.init();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);
  // lcd.font(system_5x7);
  // lcd.font(micro_5x6);
  lcd.background(Black);
  lcd.color(White);

  uint16_t i = 0;
  while (true) {
    lcd.at(28, 8);
    lcd.printf(P("Hello  World"));
    // lcd.printf(P("     Hello  World\n"));
    // lcd.printf(P("     Hello  World\n"));
    // lcd.printf(P("Hello  World\n"));

    for (char j = 0; j < 5; j++) {
      int16_t
        x = rnd() % (lcd.max_x() + 1),
        y = rnd() % (lcd.max_y() + 1);
      lcd.pixel(cycle[i][0], cycle[i][1], Black);
      lcd.pixel(x, y, 1);
      cycle[i][0] = x;
      cycle[i][1] = y;
      i++;
      if (i == COUNT_STAR) i = 0;
    }
    lcd.release();
    delay_ms(15);
  }
}
