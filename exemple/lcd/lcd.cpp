#include "config.h"
#include "SPI.h"
#include "adc.h"
#include "pinout.h"
#include "timer.h"

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

LCD lcd;
SPI spi;
ADC mic;

#define FAT           4
#define MED_FACTOR    2

#ifdef __AVR__
#define ADC_MAX       256
#else
#define ADC_MAX       4096
#endif


#define ADC_DIV       ((ADC_MAX<<1) / lcd.max_x())

void mic_view()
{
#ifdef __AVR__
  ADC5(IN);
  mic.init(5, 4);
#else
  ADC1(ANALOG);
  mic.init(1);
#endif

  mic.start();

  uint16_t x = 1;
  reg speed = 4;
  const uint16_t xx = (lcd.max_x() + 1) >> 1;
  uint16_t x2 = xx + (xx >> 1);
  uint16_t pix[lcd.max_y() + 1] = {};
  uint16_t yy = 0;
  uint16_t old = 0;
  uint16_t med = 0;

  lcd.area(xx - 1, 0, xx - 1, lcd.max_y(), RGB(32, 32, 255));

  while (true) {
    uint16_t kk = mic.value() / ADC_DIV;
    uint16_t y = x % (lcd.max_y() + 1);

    if (USER_B(GET)) {
      speed = 2;
    }
    else {
      speed = 6;
    }

    if (!(x & ((1 << speed) - 1))) {
      uint16_t y2 = (x >> speed) % (lcd.max_y() + 1);
      uint16_t k = mic.value();
      med = ((med << MED_FACTOR) - med + k) >> MED_FACTOR;
      uint16_t k2 = k > med ? (k - med) / ADC_DIV : (med - k) / ADC_DIV;
      if (k2 > (xx >> 1) - 1) k2 = -k2;
      if (k2 > (xx >> 1) - 1) k2 = (xx >> 1) - 1;
      lcd.scroll(y2 + 1);
      lcd.area(xx, y2, x2 - k2 - 1, y2, RGB(32, 32, 32));
      lcd.area(x2 - k2, y2, x2 + k2, y2, RGB(64, 255, 64));
      lcd.area(x2 + k2 + 1, y2, lcd.max_x(), y2, RGB(32, 32, 32));
    }

    lcd.color(RGB(32, 32, 32));
    lcd.w_line(pix[y] > old ? old : pix[y], y, pix[y] > old ? pix[y] : old);
    // lcd.pixel(pix[y], y);
    lcd.color(RGB(127, 255, 255));
    lcd.w_line(yy > kk ? kk : yy, y, yy > kk ? yy : kk);
    // lcd.pixel(kk, y);

    old = pix[y];
    yy = kk;
    pix[y] = kk;

    x++;
  }
}

int main(void)
{
  T32_0_PS;
  T32_0_E;
  T32_0_C;

  USER_B(GPIO);
  USER_B(IN);

  spi.init();
  lcd.init();
  lcd.background(RGB(32, 32, 32));
  lcd.color(RGB(64, 255, 64));
  lcd.clear();
  // lcd.font(arial_14);
  // lcd.font(system_5x7);
  // lcd.font(standard_5x8);
  // // lcd.font(micro_5x6);
  // lcd.font(serif_18i, 0, 0);
  lcd.font(sans_24, 0, 0);

  // mic_view();

  reg x = 1;

  while (true) {
    T32_0_C;
    if (USER_B(GET)) {
      lcd.color(~color[x & 7]);
      lcd.background(color[x++ & 7]);
      lcd.clear();
      // lcd.clear();
      // lcd.at(0, 0);
      // for (uint8_t i = serif_18i.first_char; i <= serif_18i.first_char + serif_18i.count_char; i++) {
      //   if (((i - serif_18i.first_char + 1) % 50) == 0) { while (USER_B(GET)); while (!USER_B(GET)); lcd.clear(); lcd.at(0, 0); }
      //   lcd.putc(i);
      // }
      // while (USER_B(GET));
    }
    else
      lcd.demo(x++);

    lcd.at(10, lcd.max_y() - lcd.get_height());
    uint16_t fps = (F_CPU * 10) / T32_0;
    lcd.printf(P("FPS: %u.%u"), fps / 10, fps - (fps / 10) * 10);
  }

}
