#include "core.h"
#include "lcd.h"
#include "adc.h"

RCC_ClocksTypeDef RCC_Clocks;
Pin<2, 6> USER_SW;
T32<0> tim0;
LCD lcd;

u8 *ee = (u8 *)0x1000000;
u8 *ram = (u8 *)0x2000000;
u32 *eeprom = (u32 *)0x1000000;

u32 &k = eeprom[0];

INLINE void test1(uint32_t arg)
{}

INLINE void test2(uint32_t arg)
{}

__attribute__((noinline, section(".ram_text")))
void print1(uint32_t arg)
{
  tim0.clear();
  test1(arg);
  volatile uint32_t tick = tim0.CNT() - 2;
  lcd.printf("Arg: %lu \t Time: %lu tick \n", arg, tick);
}

__attribute__((noinline, section(".ram_text")))
void print2(uint32_t arg)
{
  tim0.clear();
  test2(arg);
  volatile uint32_t tick = tim0.CNT() - 2;
  lcd.printf("Arg: %lu \t Time: %lu tick \n", arg, tick);
}

int main(void)
{
  USER_SW.init(GP_VCC);
  tim0.enable();

  RCC_GetClocksFreq(&RCC_Clocks);
  lcd.init();
  if (lcd.max_y() > 320) lcd.font(sans_24, 0, 0);
  else lcd.font(serif_18i, 0, 0);
  lcd.color(White);
  lcd.background(MidnightBlue);
  lcd.clear();

  Rect fq(220, 20, 400, 350);
  lcd.viewport(&fq);
  lcd.printf("\f");
  lcd.printf("CPU:  \t%4u\tMHz \n", RCC_Clocks.SYSCLK_Frequency / 1000000);
  lcd.printf("HCLK: \t%4u\tMHz \n", RCC_Clocks.HCLK_Frequency / 1000000);
  lcd.printf("PCLK1:\t%4u\tMHz \n", RCC_Clocks.PCLK1_Frequency / 1000000);
  lcd.printf("PCLK2:\t%4u\tMHz \n", RCC_Clocks.PCLK2_Frequency / 1000000);
  lcd.printf("ADC:  \t%4u\tMHz \n", RCC_Clocks.ADCCLK_Frequency / 1000000);
  lcd.viewport();
  lcd.printf("\f");

  // lcd.printf("\n   Test 1: \n\n");
  // for (uint32_t arg = 0; arg < 5; arg++) print1(arg);

  // lcd.printf("\n   Test 2: \n\n");
  // for (uint32_t arg = 0; arg < 5; arg++) print2(arg);

  // Rect rect(50, 100, 450, 400);

  for (int y = 0; y < 256; y++)
    for (int x = 0; x < 64; x++) {
      lcd.pixel(x + 400, y + 30, RGB(*ram, *ram << 3, *ram << 5));
      ram++;
    }

  for (int y = 0; y < 64; y++)
    for (int x = 0; x < 128; x++) {
      lcd.pixel(x + 250, y + 250, RGB(*ee, *ee << 3, *ee << 5));
      ee++;
    }

  while (true);

}
