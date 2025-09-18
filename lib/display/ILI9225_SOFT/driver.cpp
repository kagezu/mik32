#include "driver.h"

void ILI9225_SOFT::send_config(const uint16_t *config, uint8_t size)
{
  size >>= 2;
  while (size--) {
    send_command(pgm_read_word(config++));
    send_word(pgm_read_word(config++));
  }
}

void ILI9225_SOFT::init(uint8_t rotation)
{
  ILI_SOFT_SCK(GPIO); ILI_SOFT_SDA(GPIO); ILI_SOFT_RST(GPIO); ILI_SOFT_CS(GPIO); ILI_SOFT_RS(GPIO);
  ILI_SOFT_SCK(OUT); ILI_SOFT_SDA(OUT); ILI_SOFT_RST(OUT); ILI_SOFT_CS(OUT); ILI_SOFT_RS(OUT);
  ILI_SOFT_CS(SET); ILI_SOFT_RS(SET);

  ILI_SOFT_RST(CLR);     // Аппаратный сброс
  delay_ms(2);
  ILI_SOFT_RST(SET);
  delay_ms(30);
  select();
  send_config(ILI9225_CONFIG_STEP1, sizeof(ILI9225_CONFIG_STEP1));
  delay_ms(10);

  constexpr int NL_220 = 0x001C;
  constexpr int     SS = 0x0100;
  constexpr int     GS = 0x0200;
  R = rotation & EX_X_Y;

  send_command(ILI9225_DRIVER_OUTPUT_CTRL); // Отражения по осям
  send_word(
    NL_220 |
    (rotation & FLIP_X ? 0 : SS) |
    (rotation & FLIP_Y ? GS : 0)
  );
  send_command(ILI9225_ENTRY_MODE);
  send_word(0x1030 | (R >> 2));// Обмен осями
  send_config(ILI9225_CONFIG_STEP2, sizeof(ILI9225_CONFIG_STEP2));
  release();
}

void ILI9225_SOFT::send_command(uint16_t command)
{
  ILI_SOFT_RS(CLR); // Запись команды
  send_word(command);
  ILI_SOFT_RS(SET); // Запись данных
}

NOINLINE void ILI9225_SOFT::set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  // Обмен осями
  if (R) {
    send_command(ILI9225_HORIZONTAL_WINDOW_ADDR1);  send_word(y1);
    send_command(ILI9225_HORIZONTAL_WINDOW_ADDR2);  send_word(y0);
    send_command(ILI9225_VERTICAL_WINDOW_ADDR1);    send_word(x1);
    send_command(ILI9225_VERTICAL_WINDOW_ADDR2);    send_word(x0);
    send_command(ILI9225_RAM_ADDR_SET1);            send_word(y0);
    send_command(ILI9225_RAM_ADDR_SET2);            send_word(x0);
  }
  else {
    send_command(ILI9225_HORIZONTAL_WINDOW_ADDR1);  send_word(x1);
    send_command(ILI9225_HORIZONTAL_WINDOW_ADDR2);  send_word(x0);
    send_command(ILI9225_VERTICAL_WINDOW_ADDR1);    send_word(y1);
    send_command(ILI9225_VERTICAL_WINDOW_ADDR2);    send_word(y0);
    send_command(ILI9225_RAM_ADDR_SET1);            send_word(x0);
    send_command(ILI9225_RAM_ADDR_SET2);            send_word(y0);
  }

  send_command(ILI9225_GRAM_DATA_REG);
}

void ILI9225_SOFT::send_word(uint16_t rgb)
{
#ifdef __AVR__
  uint8_t d0 = ILI_SOFT_SCK(OUTPUT) & ~(ILI_SOFT_SDA(MASK) | ILI_SOFT_SCK(MASK));
  uint8_t d1 = (ILI_SOFT_SCK(OUTPUT) | ILI_SOFT_SDA(MASK)) & ~ILI_SOFT_SCK(MASK);
  uint8_t s0 = (ILI_SOFT_SCK(OUTPUT) & ~ILI_SOFT_SDA(MASK)) | ILI_SOFT_SCK(MASK);

  ILI_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;

  ILI_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;

  ILI_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;

  ILI_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;
  ILI_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
  ILI_SOFT_SCK(OUTPUT) = s0;

  ILI_SOFT_SCK(CLR);
#else    
  for (uint16_t mask = 0x8000; mask; mask >>= 1) {
    if (rgb & mask) ILI_SOFT_SDA(SET);
    else ILI_SOFT_SDA(CLR);
    ILI_SOFT_SCK(SET);
    ILI_SOFT_SCK(CLR);
  }
#endif
}

void ILI9225_SOFT::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  uint16_t rgb = color.rgb;

  ILI_SOFT_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

#ifdef __AVR__
  uint8_t d0 = ILI_SOFT_SCK(OUTPUT) & ~(ILI_SOFT_SDA(MASK) | ILI_SOFT_SCK(MASK));
  uint8_t d1 = (ILI_SOFT_SCK(OUTPUT) | ILI_SOFT_SDA(MASK)) & ~ILI_SOFT_SCK(MASK);
  uint8_t s0 = (ILI_SOFT_SCK(OUTPUT) & ~ILI_SOFT_SDA(MASK)) | ILI_SOFT_SCK(MASK);

  // Дублирование кода намеренно, так как оптимизация ускоряет тут выполнение в 2 раза
  while (len--) {
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
  }
#else
  while (len--) send_word(rgb);
#endif
  ILI_SOFT_SCK(CLR);
  ILI_SOFT_CS(SET);
}

void ILI9225_SOFT::pixel(int16_t x, int16_t y, RGB16 color)
{
  select();
  set_addr(x, y, x, y);
  send_rgb(color);
  send_rgb(color);
  release();
}
