#include "lcd.h"
#include "spi.h"
#include "pinout.h"
#include "timer32.h"

LCD lcd;
T32<0> timer;

/*
Display<ST7735_SOFT<RGB>, LCD_ROT>  lcd1;
Display<ILI9225_SOFT, LCD_ROT>      lcd3;
Display<ST7735<SPI1, RGB>, LCD_ROT> lcd2;
Display<ILI9225<SPI1>, LCD_ROT>     lcd4;
Display<SSD1306, 0>           lcd0;


int mode = 0;

template<typename T>
void init_lcd(T &lcd)
{
  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.font(arial_14, 1, 1);
  // lcd.color(White);
  // lcd.background(MidnightBlue);
}

template<typename T>
void print(T &lcd)
{
  uint32_t fps = (F_CPU << 4) / T32_0;
  // lcd0.color(White);
  // lcd0.background(Black);
  lcd0.printf(
    "\fFPS: %.2.4q\n%u X %u X %u", fps,
    lcd.max_x() + 1, lcd.max_y() + 1, RGB::len());
}


int main(void)
{
  USER_B(P_GND);

  init_lcd(lcd0);

  T32_0_PS;
  T32_0_EN;
  T32_0_C;

  int x = 0;
  while (true) {
    if (USER_B(GET)) {
      while (USER_B(GET));
      lcd0.clear();
      mode++;

      switch (mode) {
        default: mode = 1;
        case 1:
        case 2: init_lcd(lcd1); break;
        case 3:
        case 4: init_lcd(lcd2); break;
        case 5:
        case 6: init_lcd(lcd3); break;
        case 7:
        case 8: init_lcd(lcd4); break;
      }
    }

    T32_0_C;
    x++;
    switch (mode) {
      case 1: lcd1.demo(x); print(lcd1); break;
      case 2: T32_0_C; lcd1.background(color[x++ & 0x7F]);
        lcd1.clear();  print(lcd1); break;
      case 3: lcd2.demo(x); print(lcd2); break;
      case 4: T32_0_C; lcd2.background(color[x++ & 0x7F]);
        lcd2.clear();  print(lcd2); break;
      case 5: lcd3.demo(x); print(lcd3); break;
      case 6:T32_0_C; lcd3.background(color[x++ & 0x7F]);
        lcd3.clear();  print(lcd3); break;
      case 7: lcd4.demo(x); print(lcd4); break;
      case 8:T32_0_C; lcd4.background(color[x++ & 0x7F]);
        lcd4.clear();  print(lcd4); break;
    }
  }
}
*/


int mode = 1;

int main(void)
{
  USER_B(P_GND);
  lcd.init();
  if (lcd.max_y() > 300) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);

  timer.start();
  // T32_0_PS;
  // T32_0_EN;
  // T32_0_C;

  int x = 0;
  while (true) {
    if (USER_B(GET)) {
      while (USER_B(GET));
      mode++;
    }
    timer.clear();

    switch (mode) {
      case 0:
        lcd.background(color[x++ & 0x7F]);
        lcd.clear();
        break;
      case 1: lcd.demo(x++); break;
      case 2: lcd.demo2(x++); break;
      case 3: lcd.demo3(x++); break;

      default: mode = 0;
    }

    uint32_t fps = (F_CPU << 4) / timer.cnt();
    lcd.at(0, lcd.max_y() - 2 * lcd.get_height() + 1);
    lcd.color(White);
    lcd.background(MidnightBlue);
    lcd.printf(
      "FPS: %.2.4q\n%u X %u X %u", fps,
      lcd.max_x() + 1, lcd.max_y() + 1, RGB::len());
  }
}
