#include "core.h"
#include "lcd.h"
#include "adc.h"
#include "eeprom.h"
#include "load.h"

Pin<2, 6> USER_SW;
T32<0> tim0;
LCD lcd;

u8 *ee = (u8 *)0x1000000;
u8 *ram = (u8 *)0x2000000;
u32 *eeprom = (u32 *)0x1000000;

int main(void)
{
  USER_SW.init(GP_Float);
  tim0.enable();
  lcd.init();
  lcd.color(White);
  lcd.background(MidnightBlue);
  lcd.clear();

  lcd.font(serif_18i, 0, 0);
  lcd.font(*((Font *)0x01001F20), 0, 0);


  // EEPROM e;
  // e.erase(0x400, 0xFFF);
  // u32 a_off = e.write((u32)data, 0x400, sizeof(data));
  // u32 a_w = e.write((u32)offset, a_off, sizeof(offset));
  // u32 a_x = e.write((u32)width, a_w, sizeof(width));


  for (int y = 0; y < 64; y++)
    for (int x = 0; x < 128; x++) {
      lcd.pixel(x + lcd.max_x() - 140, y + 10, RGB(*ee, *ee << 3, *ee << 5));
      ee++;
    }


  while (true) {
    for (u32 *i = eeprom; i < eeprom + 0x800; i++) {
      lcd.printf("%x \t%lx    \n", (u16)(u32)i, *i);
      delay_ms(100);
      while (USER_SW.get());
    }
  }



}
