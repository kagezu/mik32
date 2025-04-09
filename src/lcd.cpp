#include "config.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"
#include "SPI.h"

#define COLORS    8

RGB color[COLORS] = {
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
SPI_Class SPI;

#ifdef __AVR__
#define USER_B(f) f(B,0)
#else
#define USER_B(f) f(2,6)
#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)

#include "adc.h"

#define FAT    4

ADC mic;

void mic_view()
{

  reg x = 1;
  reg speed = 4;
  reg xx = (lcd.max_x() + 1) >> 1;
  reg x2 = xx + (xx >> 1);
  reg pix[lcd.max_y() + 1] = {};
  reg yy = 0;
  reg old = 0;
  reg x3 = xx - FAT / 2 - 2;

  for (reg i = 0; i < FAT; i++) {
    lcd.area(x3 - i, 0, x3 - i, lcd.max_y(), RGB(32, 32, 63 + (64 >> i)));
    lcd.area(x3 + i, 0, x3 + i, lcd.max_y(), RGB(32, 32, 63 + (64 >> i)));
  }

  while (true) {
    reg kk = mic.value() / 27;
    reg y = x % (lcd.max_y() + 1);

    if (USER_B(GET)) {
      speed = 2;
    }
    else {
      speed = 4;
    }

    if (!(x & ((1 << speed) - 1))) {
      reg y2 = (x >> speed) % (lcd.max_y() + 1);
      reg k2 = kk - 70;
      if (k2 > (xx >> 1) - 1) k2 = -k2;
      if (k2 > (xx >> 1) - 1) k2 = (xx >> 1) - 1;
      // lcd.scroll(y2 + 1);
      lcd.area(xx, y2, x2 - k2 - 1, y2, RGB(32, 32, 32));
      lcd.area(x2 - k2, y2, x2 + k2, y2, RGB(64, 255, 64));
      lcd.area(x2 + k2 + 1, y2, lcd.max_x(), y2, RGB(32, 32, 32));
    }

    lcd.color(RGB(32, 32, 32));
    lcd.w_line(pix[y] > old ? old : pix[y], y, pix[y] > old ? pix[y] : old);
    lcd.color(RGB(64, 255, 64));
    lcd.w_line(yy > kk ? kk : yy, y, yy > kk ? yy : kk);

    old = pix[y];
    yy = kk;
    pix[y] = kk;

    x++;
  }
}
#endif

reg inv(reg arg)
{
  arg = (arg << 4) | (arg >> 4);
  // arg = ((arg << 2) & 0b11001100) | ((arg >> 2) & 0b00110011);
  // arg = ((arg << 1) & 0b10101010) | ((arg >> 1) & 0b01010101);

  return arg;
}

int main(void)
{
  USER_B(GPIO);
  USER_B(IN);

  // SPI.begin();
  SPI.init();
  lcd.init();
  lcd.background(RGB(32, 32, 32));
  lcd.color(RGB(64, 255, 64));
  lcd.clear();
  // lcd.font(arial_14);
  lcd.font(standard_5x8);

  // mic_view();

  reg x = 1;

  while (true) {
    if (USER_B(GET)) {
      lcd.clear(color[x % COLORS]);
    }
    else
      lcd.demo(x);

    lcd.printf(P("\f\n\n\n\n\n\n\n\n\n\n\n\n\n %u "), x++);
  }

}
