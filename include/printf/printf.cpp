#include "config.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

#define INTERLINE 3
uint32_t var;

int main(void)
{
  Display lcd;
  init_system();
  lcd.init();
  lcd.background(RGB(0, 16, 32));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);
  lcd.set_interline(INTERLINE);

  uint32_t *ptr = &var;

  while (true) {
    for (char k = lcd.get_height() + INTERLINE; k > 0; k--) {
      lcd.at(0, k);
      uint32_t *p = ptr;
      for (uint8_t i = 0; i < lcd.get_row(); i++) {
        lcd.printf(P("%p\t%10lx\t%9lx\n"), p, *p, *(p + 1));
        p += 2;
      }
    }
    ptr += 2;
  }
}
