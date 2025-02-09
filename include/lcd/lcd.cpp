#include "config.h"

int main(void)
{
  Display lcd;
  init_system();
  lcd.init();

  uint8_t x = 1;
  while (true)
    lcd.demo(x++);
}
