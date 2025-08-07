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

  ATTR_INLINE void select() { ST_8_CS(CLR); flag = 0; }

  ATTR_INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x53); // 4x4x4 bit
  }

  ATTR_INLINE void send_rgb(RGB12 color)
  {
    static uint8_t half;

  #ifdef MIK32V2
    if (flag) {
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = half | (color.rgb >> 8);
      ST_8_WR(SET);
      flag = 0;
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = color.rgb & 0xff;
      ST_8_WR(SET);
    }
    else {
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = color.rgb >> 4;
      half = color.rgb << 4;
      flag = 1;
      ST_8_WR(SET);
    }
  #else 
    if (flag) {
      ST_8_PORT(OUTPUT) = half | (color.rgb >> 8);
      ST_8_WR(SET); ST_8_WR(CLR);
      flag = 0;
      ST_8_PORT(OUTPUT) = color.rgb;
      ST_8_WR(SET); ST_8_WR(CLR);
    }
    else {
      ST_8_PORT(OUTPUT) = color.rgb >> 4;
      ST_8_WR(SET); ST_8_WR(CLR);
      half = color.rgb << 4;
      flag = 1;
    }
  #endif
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
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = hbyte;
      ST_8_WR(SET);
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = mbyte;
      ST_8_WR(SET);
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = lbyte;
      ST_8_WR(SET);
    }
  #else
    uint16_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
    while (len--) {
      ST_8_PORT(OUTPUT) = hbyte;
      ST_8_WR(SET); ST_8_WR(CLR);
      ST_8_PORT(OUTPUT) = mbyte;
      ST_8_WR(SET); ST_8_WR(CLR);
      ST_8_PORT(OUTPUT) = lbyte;
      ST_8_WR(SET); ST_8_WR(CLR);
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

  ATTR_INLINE void select() { ST_8_CS(CLR); }

  ATTR_INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x55); // 5x6x5 bit
  }

  ATTR_INLINE void send_rgb(RGB16 color)
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
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = h;
      ST_8_WR(SET);
      ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      ST_8_PORT(STATE) = l;
      ST_8_WR(SET);
    }
  #else
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ST_8_PORT(OUTPUT) = h;
        ST_8_WR(SET); ST_8_WR(CLR);
        ST_8_PORT(OUTPUT) = l;
        ST_8_WR(SET); ST_8_WR(CLR);
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

  ATTR_INLINE void select() { ST_8_CS(CLR); }

  ATTR_INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x66); // 6x6x6 bit (24 bit transfer)
  }

  ATTR_INLINE void send_rgb(RGB18 color)
  {
  #ifdef MIK32V2
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = color.red;
    ST_8_WR(SET);
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = color.green;
    ST_8_WR(SET);
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = color.blue;
    ST_8_WR(SET);
  #else
    ST_8_PORT(OUTPUT) = color.red;
    ST_8_WR(SET); ST_8_WR(CLR);
    ST_8_PORT(OUTPUT) = color.green;
    ST_8_WR(SET); ST_8_WR(CLR);
    ST_8_PORT(OUTPUT) = color.blue;
    ST_8_WR(SET); ST_8_WR(CLR);
  #endif
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    uint32_t red = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.red;
    uint32_t green = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.green;
    uint32_t blue = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.blue;
    uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      ST_8_PORT(OUTPUT) = red;
      ST_8_WR(SET);
      ST_8_PORT(OUTPUT) = green;
      ST_8_WR(SET);
      ST_8_PORT(OUTPUT) = blue;
      ST_8_WR(SET);

      // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      // ST_8_PORT(STATE) = color.red;
      // ST_8_WR(SET);
      // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      // ST_8_PORT(STATE) = color.green;
      // ST_8_WR(SET);
      // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
      // ST_8_PORT(STATE) = color.blue;
      // ST_8_WR(SET);
    }
  #else
    for (uint16_t i = y0; i <= y1; i++)
      for (uint16_t j = x0; j <= x1; j++) {
        ST_8_PORT(OUTPUT) = color.red;
        ST_8_WR(SET); ST_8_WR(CLR);
        ST_8_PORT(OUTPUT) = color.green;
        ST_8_WR(SET); ST_8_WR(CLR);
        ST_8_PORT(OUTPUT) = color.blue;
        ST_8_WR(SET); ST_8_WR(CLR);
      }
  #endif
    release();
  }
};
