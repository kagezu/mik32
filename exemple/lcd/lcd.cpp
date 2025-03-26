#include "config.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

#ifdef __AVR__
#define USER_B(f) f(B,7)
#else
#define USER_B(f) f(2,6)
#endif

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

  USER_B(GPIO);
  USER_B(IN);

  reg x = 0;
  while (true) {

    if (USER_B(GET))
      lcd.clear(RGB(-(x << 3), (x << 3), (x << 2)));
    else
      lcd.demo(x);

    lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x++);
  }

}
