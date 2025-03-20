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


  reg x = 0;
  // #define XX(x) x(0,5)

  //   XX(GPIO);
  //   XX(OUT);
  //   XX(CLR);

  // while (true) {
  //   L_PORT(MMO) = 0x0;
  //   delay_ms(1000);
  //   L_PORT(MMO) = 0xffff;
  //   delay_ms(1000);
  //   L_WR(INV); L_RS(INV); L_CS(INV);
  // }

  while (true) {
    lcd.demo(x);
    lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x++);
  }


  while (true) {
    lcd.clear(RGB(-x, x, x * x));
    x += 10;
    lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x);
  }

}
