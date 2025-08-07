#include "lcd.h"
#include "SPI.h"
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
  // lcd.font(serif_18i, 0, 0);
  lcd.background(MidnightBlue);

  int x = 0;

  while (true) {
    T32_0_C;
    if (USER_B(GET)) {
      lcd.background(color[x++ & 0x7F]);
      lcd.clear();
    }
    else {
      lcd.demo(x++);
    }

    uint16_t fps = (F_CPU << 4) / T32_0;
    lcd.at(0, lcd.max_y() - 2 * lcd.get_height() + 1);
    lcd.color(White);
    lcd.background(MidnightBlue);
    lcd.printf(
      P("FPS: %.2.4q\n%u X %u X %u"), fps,
      lcd.max_x() + 1, lcd.max_y() + 1, RGB::len()
    );
  }
}
