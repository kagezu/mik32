#include "config.h"
#include "adc.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

#ifdef __AVR__
#define USER_B(f) f(B,5)
#else
#define USER_B(f) f(2,6)
#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)
#endif

Display lcd;
ADC mic;

int main(void)
{
  init_system();

  // SPI.begin();
  lcd.init();
  lcd.background(RGB(16, 16, 16));
  lcd.color(RGB(64, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);

  USER_B(GPIO);
  USER_B(IN);

  ADC0(AN);
  ADC1(AN);
  mic.init(1, 0x3f);
  mic.start();

  reg x = 1;
  reg xx = lcd.max_x() >> 1;
  reg pix[480];

  while (true) {
    reg kk = mic.value();
    reg y = x % (lcd.max_y() + 1);
    kk /= 13;
    lcd.scroll(y + 1);
    lcd.color(RGB(32, 32, 32));
    lcd.pixel(pix[y], y);
    lcd.color(RGB(64, 255, 64));
    lcd.pixel(kk, y);
    pix[y] = kk;
    x++;
  }

  while (true) {
    // reg kk = mic.value() - 150;
    reg kk = (mic.value() - 1980);
    if (kk > 4095) kk = -kk;
    kk >>= 3;
    if (kk >= xx) kk = xx - 1;
    reg y = x % (lcd.max_y() + 1);
    lcd.scroll(y + 1);
    lcd.area(0, y, xx - kk - 1, y, RGB(32, 32, 32));
    lcd.area(xx - kk, y, xx + kk, y, RGB(64, 255, 64));
    lcd.area(xx + kk + 1, y, lcd.max_x(), y, RGB(32, 32, 32));
    x++;
    // delay_us(1000);
  }


  while (true) {
    mic.single();
    if (USER_B(GET))
      lcd.clear(RGB(-(x << 3), (x << 3), (x << 2)));
    else
      lcd.demo(x);
    lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x++);
  }

}
