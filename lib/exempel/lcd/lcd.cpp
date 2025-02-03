#include "config.h"

using namespace Core;

int main(void)
{
  CDisplay < ST7735_SPI<RGB18>, RGB18> lcd18;
  CDisplay < ST7735_SPI<RGB16>, RGB16> lcd16;
  CDisplay < ST7735_SPI<RGB12>, RGB12> lcd12;
  init_clock();
  // lcd.init();

  uint8_t x = 1;
  while (true) {

    lcd18.init();
    while (x++ & 0x20) {
      lcd18.demo(x);
    }

    lcd16.init();
    while (!(x++ & 0x20)) {
      lcd16.demo(x);
    }

    lcd12.init();
    while (x++ & 0x40) {
      lcd12.demo(x);
    }
  }
}


// lcd.clear(RGB(255, 255, 0));
// lcd.clear(RGB(0, 255, 255));
// lcd.clear(RGB(255, 0, 255));
