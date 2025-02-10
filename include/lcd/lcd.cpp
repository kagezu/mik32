#include "config.h"

volatile uint8_t f = 0;

void isr()
{
  static uint8_t t = 0;
  t++;
  if (t > 100) {
    f = 1;
    t = 0;
  }
}

int main(void)
{
  /*
  timer0(100);
  T0_CTC;
  T0_COMPA_ON;
  set_isr(TIMER0_COMPA_vect_num, isr);
*/

  Display lcd;
  init_system();
  lcd.init();

  sei();

  uint8_t x = 1;
  while (true) {
    lcd.demo(x++);
  }
}
