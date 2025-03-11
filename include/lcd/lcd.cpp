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


  uint16_t x = 0;
  while (true) {
    lcd.demo(x++);
    // delay_ms(10);
    // lcd.clear(RGB(-x, x, x * x));
    x += x % 7 + 1;
  }
}
