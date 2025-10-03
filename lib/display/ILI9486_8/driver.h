#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C>
class ILI9486_8 : public IDriver {};

////////////////////////////////// RGB16 //////////////////////////////////////

template<>
class ILI9486_8<RGB16> : public IDriver {
public:
  using RGB = RGB16;

#include "base.h"

  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x05); // 5x6x5 bit
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
      ILI_8_PORT.clr(ILI_8_MASK);
      ILI_8_PORT.set(h);
      ILI_8_WR.set();
      ILI_8_PORT.clr(ILI_8_MASK);
      ILI_8_PORT.set(l);
      ILI_8_WR.set();
    }
  #else
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ILI_8_PORT.out(h);
        ILI_8_WR.set(); ILI_8_WR.clr();
        ILI_8_PORT.out(l);
        ILI_8_WR.set(); ILI_8_WR.clr();
      }
  #endif
    release();
  }
};

////////////////////////////////// RGB18 //////////////////////////////////////

template<>
class ILI9486_8<RGB18> : public IDriver {
public:
  using RGB = RGB18;

#include "base.h"

  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x66); // 6x6x6 bit (24 bit transfer)
  }

  INLINE void send_rgb(RGB18 color)
  {
  #ifdef MIK32V2
    uint32_t mask = ILI_8_PORT.get() & ~ILI_8_MASK;
    ILI_8_PORT.out(color.red | mask);
    ILI_8_WR.set();
    ILI_8_PORT.out(color.green | mask);
    ILI_8_WR.set();
    ILI_8_PORT.out(color.blue | mask);
    ILI_8_WR.set();
  #else
    ILI_8_PORT.out(color.red);
    ILI_8_WR.set(); ILI_8_WR.clr();
    ILI_8_PORT.out(color.green);
    ILI_8_WR.set(); ILI_8_WR.clr();
    ILI_8_PORT.out(color.blue);
    ILI_8_WR.set(); ILI_8_WR.clr();
  #endif
  }

  INLINE void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    uint32_t red = (ILI_8_PORT.get() & ~ILI_8_MASK) | color.red;
    uint32_t green = (ILI_8_PORT.get() & ~ILI_8_MASK) | color.green;
    uint32_t blue = (ILI_8_PORT.get() & ~ILI_8_MASK) | color.blue;
    uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      ILI_8_PORT.out(red);
      ILI_8_WR.set();
      ILI_8_PORT.out(green);
      ILI_8_WR.set();
      ILI_8_PORT.out(blue);
      ILI_8_WR.set();
    #else
    RGB18 rgb = color.rgb24();
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ILI_8_PORT.out(rgb.red);
        ILI_8_WR.set(); ILI_8_WR.clr();
        ILI_8_PORT.out(rgb.green);
        ILI_8_WR.set(); ILI_8_WR.clr();
        ILI_8_PORT.out(rgb.blue);
        ILI_8_WR.set(); ILI_8_WR.clr();
      #endif
      }
    release();
  }
};
