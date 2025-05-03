#include "config.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "SPI.h"
#include "adc.h"

RGB color[] = {
  RGB(0,0,0),
  RGB(255,0,0),
  RGB(0,255,0),
  RGB(0,0,255),
  RGB(255,255,0),
  RGB(0,255,255),
  RGB(255,0,255),
  RGB(255,255,255)
};

Display lcd;
CSPI SPI;
ADC mic;

#ifdef __AVR__
#define USER_B(f) f(B,0)
#else
#define USER_B(f) f(2,6)
#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)

#define FAT           4
#define MED_FACTOR    6

void mic_view()
{
  ADC1(AN);
  mic.init(1);
  mic.start();

  reg x = 1;
  reg speed = 4;
  reg xx = (lcd.max_x() + 1) >> 1;
  reg x2 = xx + (xx >> 1);
  reg pix[lcd.max_y() + 1] = {};
  // reg yy = 0;
  // reg old = 0;
  reg x3 = xx - FAT / 2 - 2;
  reg med = 70 * 27;

  for (reg i = 0; i < FAT; i++) {
    lcd.area(x3 - i, 0, x3 - i, lcd.max_y(), RGB(32, 32, 63 + (64 >> i)));
    lcd.area(x3 + i, 0, x3 + i, lcd.max_y(), RGB(32, 32, 63 + (64 >> i)));
  }

  while (true) {
    reg kk = mic.value() / 27;
    reg y = x % (lcd.max_y() + 1);

    if (USER_B(GET)) {
      speed = 3;
    }
    else {
      speed = 7;
    }

    if (!(x & ((1 << speed) - 1))) {
      reg y2 = (x >> speed) % (lcd.max_y() + 1);
      // reg k2 = kk - 70;
      reg k = mic.value();
      med = ((med << MED_FACTOR) - med + k) >> MED_FACTOR;
      reg k2 = k > med ? (k - med) / 27 : (med - k) / 27;
      if (k2 > (xx >> 1) - 1) k2 = -k2;
      if (k2 > (xx >> 1) - 1) k2 = (xx >> 1) - 1;
      // lcd.scroll(y2 + 1);
      lcd.area(xx, y2, x2 - k2 - 1, y2, RGB(32, 32, 32));
      lcd.area(x2 - k2, y2, x2 + k2, y2, RGB(64, 255, 64));
      // lcd.area(x2 - k2, y2, x2 + k2, y2, color[(k2 >> 4) + 1]);
      lcd.area(x2 + k2 + 1, y2, lcd.max_x(), y2, RGB(32, 32, 32));
    }

    lcd.color(RGB(32, 32, 32));
    // lcd.w_line(pix[y] > old ? old : pix[y], y, pix[y] > old ? pix[y] : old);
    lcd.pixel(pix[y], y);
    lcd.color(RGB(127, 255, 255));
    // lcd.w_line(yy > kk ? kk : yy, y, yy > kk ? yy : kk);
    lcd.pixel(kk, y);

    // old = pix[y];
    // yy = kk;
    pix[y] = kk;

    x++;
  }
}
#endif


int main(void)
{
  USER_B(GPIO);
  USER_B(IN);

  SPI.init();
  lcd.init();
  lcd.background(RGB(32, 32, 32));
  lcd.color(RGB(64, 255, 64));
  // lcd.clear();
  lcd.font(arial_14);

  // mic_view();

  reg x = 1;

  while (true) {
    if (USER_B(GET)) {
      lcd.clear(color[x & 7]);
    }
    else
      lcd.demo(x);

    lcd.at(10, lcd.max_y() - 20);
    lcd.printf(P("%u"), x++);
  }

}
