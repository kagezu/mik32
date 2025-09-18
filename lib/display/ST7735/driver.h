#pragma once
#include "spi.h"
#include "pins.h"
#include "comon/include.h"

#define SPI_THR_2     7
#define SPI_THR_3     6

template<typename S, typename C>
class ST7735 {};

////////////////////////////////// RGB12 //////////////////////////////////////

template<typename S>
class ST7735<S, RGB12> : public IDriver {
public:
  using RGB = RGB12;
  bool flag;

#include "base.h"

  INLINE void select() { spi.begin(); ST_SPI_CS(CLR); flag = 0; }
  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x03); // 4x4x4 bit
    spi.thr(3);
  }

  void send_rgb(RGB12 color)
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

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
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
};

////////////////////////////////// RGB16 //////////////////////////////////////

template<typename S>
class ST7735<S, RGB16> : public IDriver {
public:
  using RGB = RGB16;

#include "base.h"

  INLINE void select() { spi.begin(); ST_SPI_CS(CLR); }
  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x05); // 5x6x5 bit
    spi.thr(2);
  }

  INLINE void send_rgb(RGB16 color)
  {
    spi.wait_thr();
    spi.send16(color.rgb);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) { spi.wait_thr();  spi.send16(color.rgb); }
    release();
  }
};

////////////////////////////////// RGB18 //////////////////////////////////////

template<typename S>
class ST7735<S, RGB18> : public IDriver {
public:
  using RGB = RGB18;

#include "base.h"

  INLINE void select() { spi.begin(); ST_SPI_CS(CLR); }
  INLINE   void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x06); // 6x6x6 bit (24 bit transfer)
    spi.thr(3);
  }

  INLINE void send_rgb(RGB18 color)
  {
    spi.wait_thr();
    spi.send(color.red);
    spi.wait();
    spi.send(color.green);
    spi.wait();
    spi.send(color.blue);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
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
};
