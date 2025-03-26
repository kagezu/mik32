#include "config.h"
#include "font/arial_14.h"

#define COUNT_STAR  250
int rand();

int main(void)
{
  int16_t cycle[COUNT_STAR][2];
  RGB black(0, 16, 32);
  RGB white(255, 255, 96);
  Display lcd;
  init_system();
  lcd.init();
  lcd.background(black);
  lcd.color(white);
  lcd.clear();
  lcd.font(arial_14);

  uint16_t i = 0;
  while (true) {
    lcd.at((MAX_X - lcd.get_weight() * 10) >> 1, (MAX_Y - lcd.get_height()) >> 1);
    lcd.color(white);
    lcd.printf(P("Hello  World"));
    for (char j = 0; j < 5; j++) {
      int16_t x = rand() % MAX_X, y = rand() % MAX_Y;
      lcd.color(black);
      lcd.pixel(cycle[i][0], cycle[i][1]);
      lcd.color(RGB(rand()));
      lcd.pixel(x, y);
      cycle[i][0] = x;
      cycle[i][1] = y;
      i++;
      if (i == COUNT_STAR) i = 0;
    }
    delay_ms(15);
  }
}


static unsigned long int next = 1;
int rand()
{
  next = next * 1103515245 + 12345;
  return (unsigned int)(next >> 8);
}
