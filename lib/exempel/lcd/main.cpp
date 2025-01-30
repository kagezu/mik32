#include "lcd.h"

int main(void)
{
  Display lcd;
  init_clock(); // Включение тактирования GPIO
  lcd.init();
  uint8_t x = 0;
  while (true) {
    // L_RS(SET); // Запись данных
    lcd.demo(x++);
  }
}
