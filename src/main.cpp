#define RGB           RGB16
#define LCD_DRIVER    ST7735_SOFT
// #define LCD_FLIP      EX_X_Y | FLIP_X
#include "display.h"


uint8_t x = 0;
int main(void)
{
  Display lcd;

  init_clock(); // Включение тактирования GPIO
  lcd.init();
  while (true) {
    // L_RS(SET); // Запись данных
    lcd.demo(x++);
    // lcd.clear(RGB(100, 200, 255));
    // lcd.clear(RGB(255, 100, 0));
    // lcd.clear();
  }
}
