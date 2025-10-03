#pragma once
#include "pins.h"
#include "ILI9225/const.h"
#include "comon/include.h"

class ILI9225_SOFT {
private:
  uint8_t R;
  void send_config(const uint16_t *config, uint8_t size);

public:
  using RGB = RGB16; // Только 16 бит
  void init(uint8_t rotation = 0);
  INLINE constexpr int16_t max_x() { return 175; }
  INLINE constexpr int16_t max_y() { return 219; }
  INLINE void select() { ILI_SOFT_CS.clr(); }
  INLINE void release() { ILI_SOFT_SCK.clr(); ILI_SOFT_CS.set(); }
  INLINE void send_rgb(RGB16 color, uint8_t len) { while (len--)send_rgb(color); }
  INLINE void send_rgb(RGB16 color) { send_word(color.rgb); }

  void send_command(uint16_t command);
  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void send_word(uint16_t rgb);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color);
  void pixel(int16_t x, int16_t y, RGB16 color);
};

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
  ILI_SOFT_SCK.init(GPO_2mA);
  ILI_SOFT_SDA.init(GPO_2mA);
  ILI_SOFT_RST.init(GPO_2mA);
  ILI_SOFT_CS.init(GPO_2mA);
  ILI_SOFT_RS.init(GPO_2mA);
  ILI_SOFT_CS.set(); ILI_SOFT_RS.set();

  ILI_SOFT_RST.clr();     // Аппаратный сброс
  delay_ms(2);
  ILI_SOFT_RST.set();
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
  ILI_SOFT_RS.clr(); // Запись команды
  send_word(command);
  ILI_SOFT_RS.set(); // Запись данных
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
  for (uint16_t mask = 0x8000; mask; mask >>= 1) {
    if (rgb & mask) ILI_SOFT_SDA.set();
    else ILI_SOFT_SDA.clr();
    ILI_SOFT_SCK.set();
    ILI_SOFT_SCK.clr();
  }
}

void ILI9225_SOFT::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  uint16_t rgb = color.rgb;

  ILI_SOFT_CS.clr();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) send_word(rgb);

  ILI_SOFT_SCK.clr();
  ILI_SOFT_CS.set();
}

void ILI9225_SOFT::pixel(int16_t x, int16_t y, RGB16 color)
{
  select();
  set_addr(x, y, x, y);
  send_rgb(color);
  send_rgb(color);
  release();
}
