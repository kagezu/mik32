#include "lcd.h"

using namespace Core;

int main(void)
{
  Display lcd;
  init_clock(); // Включение тактирования GPIO
  lcd.init();
  uint8_t x = 0;
  while (true) {
    lcd.demo(x++);
    // lcd.clear(RGB(255, 255, 0));
    // lcd.clear(RGB(0, 255, 255));
    // lcd.clear(RGB(255, 0, 255));
  }
}
