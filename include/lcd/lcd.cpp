#include "config.h"

Display *lcd;

void isr()
{
  static uint8_t x = 1;
  static uint8_t t = 0;
  t++;
  if (t > 100) {
    t = 0;
    // lcd->demo(x++);
  }
}

int main(void)
{
  timer0(100);
  T0_CTC;
  T0_COMPA_ON;
  set_isr(TIMER0_COMPA_vect_num, isr);

  Display disp;
  lcd = &disp;
  init_system();
  lcd->init();

  sei();

  uint8_t x = 1;
  while (true) {
    lcd->demo(x++);
  }
}
