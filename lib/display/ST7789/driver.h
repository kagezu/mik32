#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C>
class ST7789 : public IDriver {
private:
  ATTR_INLINE void set_rgb_format();
  uint8_t flag = 0;

public:
  // Разрешение дисплея
  ATTR_INLINE constexpr int16_t max_x() { return 239; }
  ATTR_INLINE constexpr int16_t max_y() { return 319; }
  using RGB = C;

  void init(uint8_t position = 0)
  {
    ST_8_RD(GPIO); ST_8_WR(GPIO); ST_8_RS(GPIO); ST_8_CS(GPIO); ST_8_RST(GPIO);
    ST_8_RD(OUT); ST_8_WR(OUT); ST_8_RS(OUT); ST_8_CS(OUT); ST_8_RST(OUT);
    ST_8_PORT(OUT) | 0xFF;
    ST_8_RD(SET); ST_8_WR(CLR); ST_8_RS(CLR); ST_8_CS(SET);
    ST_8_RST(CLR);
    ST_8_RST(SET);

    select();
    send_command(SWRESET);
    delay_ms(25);

    send_config(ST7789_CONFIG, sizeof(ST7789_CONFIG));
    send_command(MADCTL); send_byte(position);
    set_rgb_format();
    send_command(SLPOUT);
    send_command(NORON);
    send_command(DISPON);
    release();
  }

protected:
  ATTR_INLINE  void select() { ST_8_CS(CLR); }
  ATTR_INLINE  void release() { ST_8_CS(SET); }

  ATTR_INLINE void send_command(uint8_t command)
  {
    ST_8_RS(CLR);
    send_byte(command);
    ST_8_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data)
  {
  #ifdef MIK32V2
    ST_8_PORT(OUTPUT) = data | (ST_8_PORT(OUTPUT) & ~0xff);
    ST_8_WR(CLR); ST_8_WR(SET);
  #else
    ST_8_PORT(OUTPUT) = data;
    ST_8_WR(SET); ST_8_WR(CLR);
  #endif
  }

  ATTR_INLINE void send_word(uint16_t data)
  {
  #ifdef MIK32V2
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = data >> 8;
    ST_8_WR(SET);
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = data & 0xff;
    ST_8_WR(SET);
  #else
    ST_8_PORT(OUTPUT) = to_byte(data, 1);
    ST_8_WR(SET); ST_8_WR(CLR);
    ST_8_PORT(OUTPUT) = to_byte(data, 0);
    ST_8_WR(SET); ST_8_WR(CLR);
  #endif
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); send_word(x0); send_word(x1);
    send_command(RASET); send_word(y0); send_word(y1);
    send_command(RAMWR);
  }

  ATTR_INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }
  ATTR_INLINE void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);
};

#include "rgb12.tpp"
#include "rgb16.tpp"
#include "rgb18.tpp"
