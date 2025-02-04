#include "config.h"

using namespace Core;

int main(void)
{
  Display lcd;
  init_clock();
  lcd.init();

  uint8_t x = 1;
  while (true)
    lcd.demo(x++);
}
