#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C>
class ILI9486_8 : public IDriver {
private:
  ATTR_INLINE void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 319; }
  ATTR_INLINE constexpr int16_t max_y() { return 479; }
  ATTR_INLINE  void select() { ILI_8_CS(CLR); }
  ATTR_INLINE  void release() { ILI_8_CS(SET); }
  ATTR_INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }

  ATTR_INLINE void send_rgb(C color);
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

  ATTR_INLINE void send_command(uint8_t command)
  {
    ILI_8_RS(CLR);
    send_byte(command);
    ILI_8_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data)
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

  ATTR_INLINE void send_word(uint16_t data)
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

#include "rgb16.tpp"
#include "rgb18.tpp"
