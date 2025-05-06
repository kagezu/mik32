#include "config.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "SPI.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"

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
ADC adc;
DMA dma(0);

#define USER_B(f) f(2,6)
#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)

#define CYCLES  1000

// reg buffer[CYCLES];

void GCC_RAM test()
{
  ADC0(AN);
  // ADC1(AN);// ADC1(OUT); ADC1(CLR);
  T32_0_POWER_ON;
  T32_0_ON;

  for (uint32_t i = 2; i < 25; i += 2) {
    adc.init(0, i);
    adc.start();
    T32_0_CLR;
    T32_0_ON;
    reg value = adc.value();
    for (reg n = 1; n < CYCLES; n++) {
      // ADC1(INV);
      while (value == adc.value());
      value = adc.value();
    }
    reg time = T32_0;
    adc.stop();
    // delay_ms(300);
    // reg count = 0;
    // reg value = buffer[0];
    // for (reg n = 1; n < CYCLES; n++) {
    //   if (value != buffer[n]) {
    //     count++;
    //     value = buffer[n];
    //   }
    // }

    lcd.printf("%2lu %5lu %5lu \n", i, time / CYCLES, time);
  }
  T32_0_OFF;
  T32_0_POWER_OFF;
}

int main(void)
{
  USER_B(GPIO);
  USER_B(IN);

  SPI.init();
  lcd.init();
  lcd.background(RGB(32, 32, 32));
  lcd.color(RGB(64, 255, 64));
  lcd.clear();
  lcd.font(standard_5x8);
  // lcd.font(arial_14);


  // reg x = 1;

  test();
  while (true) {
    //   if (USER_B(GET)) {
    //     lcd.clear(color[x & 7]);
    //   }
    //   else
    //     lcd.demo(x);

    //   lcd.at(10, lcd.max_y() - 20);
    //   lcd.printf(P("%u"), x++);
  }

}
