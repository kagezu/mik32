#include "lcd.h"
#include "font/arial_14.h"

#define COUNT_STAR  150
using namespace Core;
int rand();

int main(void)
{
  int16_t cycle[COUNT_STAR][2];
  RGB black(0, 16, 32);
  RGB white(255, 255, 64);
  Display lcd;
  init_clock();
  lcd.init();
  lcd.background(RGB(0, 24, 48));
  lcd.color(RGB(255, 255, 96));
  lcd.clear();
  lcd.font(arial_14);

  uint16_t i = 0;
  while (true) {
    lcd.at((MAX_Y - lcd.get_weight() * 5) >> 1, (MAX_Y - lcd.get_height()) >> 1);
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
    delay_ms(5);
  }
}


static unsigned long int next = 1;
int rand()
{
  next = next * 1103515245 + 12345;
  return (unsigned int)(next >> 8);
}
