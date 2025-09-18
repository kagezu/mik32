#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C>
class ILI9486_8 : public IDriver {
private:
  INLINE void set_rgb_format();

public:
  using RGB = C;
  INLINE constexpr int16_t max_x() { return 319; }
  INLINE constexpr int16_t max_y() { return 479; }
  INLINE  void select() { ILI_8_CS(CLR); }
  INLINE  void release() { ILI_8_CS(SET); }
  INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }

  INLINE void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0)
  {
    ILI_8_RD(GPIO); ILI_8_WR(GPIO); ILI_8_RS(GPIO); ILI_8_CS(GPIO); ILI_8_RST(GPIO);
    ILI_8_RD(OUT); ILI_8_WR(OUT); ILI_8_RS(OUT); ILI_8_CS(OUT); ILI_8_RST(OUT);
    ILI_8_PORT(OUT) | 0xFF;
    ILI_8_RD(SET); ILI_8_WR(CLR); ILI_8_RS(CLR); ILI_8_CS(SET); ILI_8_RST(CLR);
    ILI_8_RST(SET);

    select();
    send_command(SLPOUT);
    delay_ms(50);
    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL); send_byte((rotation | 0x08));// BGR -> RBG
    set_rgb_format();
    delay_ms(50);
    send_command(NORON);
    send_command(DISPON);
    release();
  }

  void scroll(uint16_t sl)
  {
    select();
    send_command(VSCRSADD);
    send_word(sl);
    release();
  }

  INLINE void send_command(uint8_t command)
  {
    ILI_8_RS(CLR);
    send_byte(command);
    ILI_8_RS(SET);
  }

  INLINE void send_byte(uint8_t data)
  {
  #ifdef MIK32V2
    ILI_8_WR(CLR) | 0xff;
    ILI_8_PORT(SET) | data;
    ILI_8_WR(SET);
  #else
    ILI_8_PORT(OUTPUT) = data;
    ILI_8_WR(SET); ILI_8_WR(CLR);
  #endif
  }

  INLINE void send_word(uint16_t data)
  {
  #ifdef MIK32V2
    ILI_8_PORT(CLR) | 0xff | ILI_8_WR(MASK);
    ILI_8_PORT(STATE) = data >> 8;
    ILI_8_WR(SET);
    ILI_8_PORT(CLR) | 0xff | ILI_8_WR(MASK);
    ILI_8_PORT(STATE) = data & 0xff;
    ILI_8_WR(SET);
  #else
    ILI_8_PORT(OUTPUT) = data >> 8;
    ILI_8_WR(SET); ILI_8_WR(CLR);
    ILI_8_PORT(OUTPUT) = data;
    ILI_8_WR(SET); ILI_8_WR(CLR);
  #endif
  }

  void pixel(int16_t x, int16_t y, C color)
  {
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }
};

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
      ILI_8_PORT(CLR) | 0xff | ILI_8_WR(MASK);
      ILI_8_PORT(STATE) = h;
      ILI_8_WR(SET);
      ILI_8_PORT(CLR) | 0xff | ILI_8_WR(MASK);
      ILI_8_PORT(STATE) = l;
      ILI_8_WR(SET);
    }
  #else
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ILI_8_PORT(OUTPUT) = h;
        ILI_8_WR(SET); ILI_8_WR(CLR);
        ILI_8_PORT(OUTPUT) = l;
        ILI_8_WR(SET); ILI_8_WR(CLR);
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
    uint32_t mask = ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK));
    ILI_8_PORT(OUTPUT) = color.red | mask;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = color.green | mask;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = color.blue | mask;
    ILI_8_WR(SET);
  #else
    ILI_8_PORT(OUTPUT) = color.red;
    ILI_8_WR(SET); ILI_8_WR(CLR);
    ILI_8_PORT(OUTPUT) = color.green;
    ILI_8_WR(SET); ILI_8_WR(CLR);
    ILI_8_PORT(OUTPUT) = color.blue;
    ILI_8_WR(SET); ILI_8_WR(CLR);
  #endif
  }

  INLINE void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    uint32_t red = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.red;
    uint32_t green = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.green;
    uint32_t blue = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.blue;
    uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      ILI_8_PORT(OUTPUT) = red;
      ILI_8_WR(SET);
      ILI_8_PORT(OUTPUT) = green;
      ILI_8_WR(SET);
      ILI_8_PORT(OUTPUT) = blue;
      ILI_8_WR(SET);
    #else
    RGB18 rgb = color.rgb24();
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ILI_8_PORT(OUTPUT) = rgb.red;
        ILI_8_WR(SET); ILI_8_WR(CLR);
        ILI_8_PORT(OUTPUT) = rgb.green;
        ILI_8_WR(SET); ILI_8_WR(CLR);
        ILI_8_PORT(OUTPUT) = rgb.blue;
        ILI_8_WR(SET); ILI_8_WR(CLR);
      #endif
    }
    release();
  }
};
