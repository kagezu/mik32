#include "config.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

Display lcd;

int main(void)
{
  init_system();
  // SPI.begin();
  lcd.init();
  lcd.background(RGB(0, 16, 32));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);


  reg x = 0;

  // while (true) {
  //   lcd.demo(x);
  //   lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x++);
  // }


  while (true) {
    lcd.clear(RGB(-x, x, x * x));
    // x += x % 7 + 1;
    lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x++);
  }

}
