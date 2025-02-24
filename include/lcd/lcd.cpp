#include "config.h"
#include "VS1053/VS1053.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

Display lcd;
VS1053 midi;

int main(void)
{
  init_system();
  SPI.begin();
  lcd.init();
  midi.init();
  lcd.background(RGB(0, 16, 32));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);

  midi.pgm_change(30);

  lcd.printf(P("\f  Started"));

  uint8_t x = 40;
  while (true) {
    x++;
    if (x > 120) x = 10;
    midi.note_on(x);
    // lcd.demo(x++);
    delay_ms(100);
    midi.note_off(x);
    lcd.printf(P("\f\n\n  %x"), midi.read_register(SCI_STATUS));
    lcd.printf(P("\n  %x"), midi.read_register(SCI_VOL));
    lcd.printf(P("\n  %u   "), x);
  }
}
