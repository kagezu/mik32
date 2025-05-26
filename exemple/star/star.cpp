#include "config.h"

#define COUNT_STAR  250

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
  RGB black(0, 16, 32);
  RGB white(255, 255, 96);

  lcd.init();
  lcd.background(black);
  lcd.color(white);
  lcd.clear();
  // lcd.font(arial_14);
  // lcd.font(micro_5x6);
  // lcd.font(system_5x7);
  lcd.font(standard_5x8);

  uint16_t i = 0;
  while (true) {
    // lcd.at((lcd.max_x() - 80) >> 1, (lcd.max_y() - 40) >> 1);
    lcd.font(arial_14);
    lcd.printf(P("\fHello  World\n"));
    lcd.font(standard_5x8);
    lcd.printf(P("Hello  World\n"));
    lcd.font(system_5x7);
    lcd.printf(P("Hello  World\n"));
    lcd.font(micro_5x6);
    lcd.printf(P("Hello  World\n"));
    for (char j = 0; j < 5; j++) {
      int16_t x = rnd() % (lcd.max_y() + 1), y = rnd() % (lcd.max_y() + 1);
      lcd.pixel(cycle[i][0], cycle[i][1], black);
      lcd.pixel(x, y, RGB(rnd()));
      cycle[i][0] = x;
      cycle[i][1] = y;
      i++;
      if (i == COUNT_STAR) i = 0;
    }
    delay_ms(15);
  }
}
