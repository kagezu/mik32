#pragma once
#include "SPI.h"
#include "pins.h"
#include "comon/include.h"

#define SPI_THR_2     7
#define SPI_THR_3     6

template<typename S = SPI1, typename C = RGB16>
class ST7735 {
public:
  S spi;
  SPIConf spi_conf;

  // Разрешение дисплея
  ATTR_INLINE constexpr int16_t max_x() { return 127; }
  ATTR_INLINE constexpr int16_t max_y() { return 159; }

  void init(uint8_t position = 0)
  {
    ST_SPI_RST(OUT); ST_SPI_RST(CLR);
    ST_SPI_CS(OUT); ST_SPI_CS(SET);
    ST_SPI_RS(OUT); ST_SPI_RS(SET);
    ST_SPI_RST(SET);

    // Установка порога для буфера
    spi_conf.thr(sizeof(C) == 2 ? SPI_THR_2 : SPI_THR_3);
    // Настройка для инициализации
    spi_conf.mode(SPI_MODE0);

    select();               // CS Выбор дисплея
    send_command(SWRESET);
    delay_ms(50);
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(position);
    set_rgb_format();
    send_command(DISPON);   // Display On
    release();

    // Ускоряет ещё на 11,7% для MIK32
    spi_conf.mode(SPI_MODE3);
  }

protected:
  ATTR_INLINE void select() { spi.begin(spi_conf); ST_SPI_CS(CLR); flag = 0; }
  ATTR_INLINE void release() { spi.end(); ST_SPI_CS(SET); }

  ATTR_INLINE void send_byte(uint8_t data) { spi.send(data); spi.wait_idle(); }
  ATTR_INLINE void send_command(uint8_t command)
  {
    ST_SPI_RS(CLR); // Запись команды
    send_byte(command);
    ST_SPI_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); // Column Address Set
    spi.send16(x0);
    spi.wait();
    spi.send16(x1);
    spi.wait_idle();

    send_command(RASET); // Row Address Set
    spi.send16(y0);
    spi.wait();
    spi.send16(y1);
    spi.wait_idle();

    send_command(RAMWR); // Memory Write
  }

  ATTR_INLINE void send_rgb(C color, uint8_t len) { while (len--)send_rgb(color); }

  ATTR_INLINE void send_rgb(C color)
  {
    spi.wait_thr();
    spi.send(color.red);
    spi.wait();
    spi.send(color.green);
    spi.wait();
    spi.send(color.blue);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      spi.wait_thr();
      spi.send(color.red);
      spi.wait();
      spi.send(color.green);
      spi.wait();
      spi.send(color.blue);
    }
    release();
  }

private:
  ATTR_INLINE void set_rgb_format();
  virtual void send_config(const uint8_t *config, uint8_t size) = 0;
  uint8_t flag = 0;
};

////////////////////////////// SPI 0 //////////////////////////////////////////

template<>
ATTR_INLINE void ST7735<SPI0, RGB16>::send_rgb(RGB16 color)
{
  spi.wait_thr();
  spi.send16(color.rgb);
}

template<>
ATTR_INLINE  void ST7735<SPI0, RGB12>::send_rgb(RGB12 color)
{
  static uint8_t half;

  if (flag) {
    spi.wait_thr();
    spi.send(half | (color.rgb >> 8));
    flag = 0;
    spi.wait();
    spi.send(color.rgb);
  }
  else {
    spi.send(color.rgb >> 4);
    half = color.rgb << 4;
    flag = 1;
  }
}

template<>
void ST7735<SPI0, RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) { spi.wait_thr();  spi.send16(color.rgb); }
  release();
}

template<>
void ST7735<SPI0, RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;

  uint8_t hbyte = color.rgb >> 4;
  uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
  uint8_t lbyte = color.rgb;

  while (len--) {
    spi.wait_thr();
    spi.send(hbyte);
    spi.wait();
    spi.send(mbyte);
    spi.wait();
    spi.send(lbyte);
  }
  release();
}

template<>
void ST7735<SPI0, RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  spi_conf.thr(SPI_THR_3);
}
template<>
void ST7735<SPI0, RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  spi_conf.thr(SPI_THR_3);
}
template<>
void ST7735<SPI0, RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
  spi_conf.thr(SPI_THR_2);
}
template<>
void ST7735<SPI0, RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
  spi_conf.thr(SPI_THR_3);
}

////////////////////////////// SPI 1 //////////////////////////////////////////

template<>
ATTR_INLINE void ST7735<SPI1, RGB16>::send_rgb(RGB16 color)
{
  spi.wait_thr();
  spi.send16(color.rgb);
}

template<>
ATTR_INLINE void ST7735<SPI1, RGB12>::send_rgb(RGB12 color)
{
  static uint8_t half;

  if (flag) {
    spi.wait_thr();
    spi.send(half | (color.rgb >> 8));
    flag = 0;
    spi.wait();
    spi.send(color.rgb);
  }
  else {
    spi.send(color.rgb >> 4);
    half = color.rgb << 4;
    flag = 1;
  }
}

template<>
void ST7735<SPI1, RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) { spi.wait_thr();  spi.send16(color.rgb); }
  release();
}

template<>
void ST7735<SPI1, RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;

  uint8_t hbyte = color.rgb >> 4;
  uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
  uint8_t lbyte = color.rgb;

  while (len--) {
    spi.wait_thr();
    spi.send(hbyte);
    spi.wait();
    spi.send(mbyte);
    spi.wait();
    spi.send(lbyte);
  }
  release();
}

template<>
void ST7735<SPI1, RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  spi_conf.thr(SPI_THR_3);
}
template<>
void ST7735<SPI1, RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  spi_conf.thr(SPI_THR_3);
}
template<>
void ST7735<SPI1, RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
  spi_conf.thr(SPI_THR_2);
}
template<>
void ST7735<SPI1, RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
  spi_conf.thr(SPI_THR_3);
}
