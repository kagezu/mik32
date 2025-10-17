#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C>
class ST7789 : public IDriver {};


////////////////////////////////// RGB12 //////////////////////////////////////

template<>
class ST7789<RGB12> : public IDriver {
private:
  uint8_t flag = 0;

public:
  using RGB = RGB12;

#include "base.h"

  INLINE void select() { ST_8_CS.clr(); flag = 0; }

  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x53); // 4x4x4 bit
  }

  INLINE void send_rgb(RGB12 color)
  {
    static uint8_t half;

    if (flag) {
      ST_8_PORT.out(half | (color.rgb >> 8));
      ST_8_WR.set(); ST_8_WR.clr();
      flag = 0;
      ST_8_PORT.out(color.rgb);
      ST_8_WR.set(); ST_8_WR.clr();
    }
    else {
      ST_8_PORT.out(color.rgb >> 4);
      ST_8_WR.set(); ST_8_WR.clr();
      half = color.rgb << 4;
      flag = 1;
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

    uint8_t hbyte = color.rgb >> 4;
    uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
    uint8_t lbyte = color.rgb;

  #ifdef MIK32V2
    uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;

    while (len--) {
      ST_8_PORT.out(hbyte);
      ST_8_WR.set(); ST_8_WR.clr();
      ST_8_PORT.out(mbyte);
      ST_8_WR.set(); ST_8_WR.clr();
      ST_8_PORT.out(lbyte);
      ST_8_WR.set(); ST_8_WR.clr();
    }
  #else
    uint16_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
    while (len--) {
      ST_8_PORT.out(hbyte);
      ST_8_WR.set(); ST_8_WR.clr();
      ST_8_PORT.out(mbyte);
      ST_8_WR.set(); ST_8_WR.clr();
      ST_8_PORT.out(lbyte);
      ST_8_WR.set(); ST_8_WR.clr();
    }
  #endif
    release();
  }
};

////////////////////////////////// RGB16 //////////////////////////////////////

template<>
class ST7789<RGB16> : public IDriver {
public:
  using RGB = RGB16;

#include "base.h"

  INLINE void select() { ST_8_CS.clr(); }

  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x55); // 5x6x5 bit
  }

  INLINE void send_rgb(RGB16 color)
  {
    send_word(color.rgb);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint8_t h = color.rgb >> 8;
    uint8_t l = color.rgb & 0xff;

  #ifdef MIK32V2
    uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
    while (len--) {
      ST_8_PORT.out(h);
      ST_8_WR.set(); ST_8_WR.clr();
      ST_8_PORT.out(l);
      ST_8_WR.set(); ST_8_WR.clr();
    }
  #else
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ST_8_PORT.out(h);
        ST_8_WR.set(); ST_8_WR.clr();
        ST_8_PORT.out(l);
        ST_8_WR.set(); ST_8_WR.clr();
      }
  #endif
    release();
  }
};

////////////////////////////////// RGB18 //////////////////////////////////////

template<>
class ST7789<RGB18> : public IDriver {
public:
  using RGB = RGB18;

#include "base.h"

  INLINE void select() { ST_8_CS.clr(); }

  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x66); // 6x6x6 bit (24 bit transfer)
  }

  INLINE void send_rgb(RGB18 color)
  {
    ST_8_PORT.out(color.red);
    ST_8_WR.set(); ST_8_WR.clr();
    ST_8_PORT.out(color.green);
    ST_8_WR.set(); ST_8_WR.clr();
    ST_8_PORT.out(color.blue);
    ST_8_WR.set(); ST_8_WR.clr();
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      ST_8_PORT.out(color.red);
      ST_8_WR.set();
      ST_8_PORT.out(color.green);
      ST_8_WR.set();
      ST_8_PORT.out(color.blue);
      ST_8_WR.set();

      // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      // ST_8_PORT(STATE) = color.red;
      // ST_8_WR.set();
      // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      // ST_8_PORT(STATE) = color.green;
      // ST_8_WR.set();
      // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      // ST_8_PORT(STATE) = color.blue;
      // ST_8_WR.set();
    }
  #else
    for (uint16_t i = y0; i <= y1; i++)
      for (uint16_t j = x0; j <= x1; j++) {
        ST_8_PORT.out(color.red);
        ST_8_WR.set(); ST_8_WR.clr();
        ST_8_PORT.out(color.green);
        ST_8_WR.set(); ST_8_WR.clr();
        ST_8_PORT.out(color.blue);
        ST_8_WR.set(); ST_8_WR.clr();
      }
  #endif
    release();
  }
};
