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


  uint8_t x = 40;
  while (true) {
    lcd.demo(x);
    x += 2;
    // lcd.clear(RGB(255 - x, x - 127, x += 4));
  }
}
