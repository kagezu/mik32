#include "config.h"
// #include "VS1053/VS1053.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

#define INTERLINE 3
using namespace Core;

int main(void)
{
  Display lcd;
  init_clock();
  lcd.init();
  lcd.background(RGB(0, 16, 32));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);
  lcd.set_interline(INTERLINE);

  reg t = 0;
  while (true) {
    lcd.printf(P("\f%u                \n\n"), t);
    lcd.printf(P("\t%u   %u            "), t / 60, t % 60);
    t++;
    delay_ms(1000);
  }
}
