#include "lcd.h"
#include "SPI.h"
#include "adc.h"
#include "pinout.h"
#include "timer.h"

LCD lcd;

int main(void)
{
  T32_0_PS;
  T32_0_EN;
  T32_0_C;

  USER_B(GPIO);
  USER_B(IN);

  lcd.init();
  lcd.font(sans_24, 0, 0);

  int x = 0;

  while (true) {
    T32_0_C;
    if (USER_B(GET)) {
      lcd.color(color[x & 0x7F]);
      lcd.background(~color[x++ & 0x7F]);
      lcd.clear();
    }
    else
      lcd.demo(x++);

    uint16_t fps = (F_CPU << 4) / T32_0;
    lcd.at(10, lcd.max_y() - lcd.get_height());
    lcd.printf(P("FPS: %.2.4q"), fps);
  }
}
