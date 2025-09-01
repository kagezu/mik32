#pragma once
#include "spi.h"
#include "pins.h"
#include "const.h"
#include "comon/include.h"

template<typename S>
class ILI9225 {
private:
  uint8_t R;
  S spi;

  void send_config(const uint16_t *config, uint8_t size)
  {
    size >>= 2;
    while (size--) {
      send_command(pgm_read_word(config++));
      send_word(pgm_read_word(config++));
    }
  }

public:
  using RGB = RGB16; // Только 16 бит
  ATTR_INLINE constexpr int16_t max_x() { return 175; }
  ATTR_INLINE constexpr int16_t max_y() { return 219; }
  ATTR_INLINE void select() { spi.begin(); ILI_SPI_CS(CLR); }
  ATTR_INLINE void release() { spi.end(); ILI_SPI_CS(SET); }
  ATTR_INLINE void send_byte(uint8_t data) { spi.send(data); spi.wait_idle(); }
  ATTR_INLINE void send_word(uint16_t data) { spi.send16(data); spi.wait_idle(); }
  ATTR_INLINE void send_rgb(RGB16 color, uint8_t len) { while (len--)send_rgb(color); }
  ATTR_INLINE void send_rgb(RGB16 color) { send_word(color.rgb); }

  void init(uint8_t rotation)
  {
    ILI_SPI_RST(GPIO); ILI_SPI_CS(GPIO); ILI_SPI_RS(GPIO);
    ILI_SPI_RST(OUT); ILI_SPI_CS(OUT); ILI_SPI_RS(OUT);
    ILI_SPI_CS(SET); ILI_SPI_RS(SET);

    ILI_SPI_RST(CLR);     // Аппаратный сброс
    delay_ms(2);
    ILI_SPI_RST(SET);
    delay_ms(30);

    constexpr int SPI_THR_2 = 7;

    // Установка порога для буфера
    spi.thr(SPI_THR_2);
    spi.mode(SPI_MODE0); // Настройка для инициализации

    select();
    send_config(ILI9225_CONFIG_STEP0, sizeof(ILI9225_CONFIG_STEP0));
    delay_ms(10);
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

    spi.mode(SPI_MODE3);// Ускоряет ещё на 11,7% для MIK32
  }

  void send_command(uint16_t command)
  {
    ILI_SPI_RS(CLR); // Запись команды
    send_word(command);
    ILI_SPI_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
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

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) { spi.wait_thr();  spi.send16(color.rgb); }
    release();
  }

  void pixel(int16_t x, int16_t y, RGB16 color)
  {
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }
};

