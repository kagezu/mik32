#include "config.h"
#include "VS1053/VS1053.h"

int main(void)
{
  Display lcd;
  VS1053 midi;
  Core::init_clock();
  lcd.init();
  midi.init();




  uint8_t x = 1;
  while (true)
    lcd.demo(x++);
}
