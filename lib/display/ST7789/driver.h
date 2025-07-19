#pragma once
#include "pins.h"
#include "type/include.h"

// #define ST_8_WRITE  ST_8_RD(SET); ST_8_PORT(OUT) | 0xFF;
// #define ST_8_READ   ST_8_PORT(IN) & 0x00; ST_8_RD(CLR);

template<typename C = RGB32>
class ST7789 {
public:
  // Разрешение дисплея
  ATTR_INLINE constexpr int16_t max_x() { return 239; }
  ATTR_INLINE constexpr int16_t max_y() { return 319; }

  void init(uint8_t position = 0)
  {
    // ST_8_RD(GPIO); ST_8_WR(GPIO); ST_8_RS(GPIO); ST_8_CS(GPIO); ST_8_RST(GPIO);
    ST_8_RD(OUT); ST_8_WR(OUT); ST_8_RS(OUT); ST_8_CS(OUT); ST_8_RST(OUT);
    ST_8_PORT(OUT) | 0xFF;
    ST_8_RD(SET); ST_8_WR(CLR); ST_8_RS(CLR); ST_8_CS(SET); ST_8_RST(CLR);
    ST_8_RST(SET);

    select();             // CS Выбор дисплея
    send_command(SWRESET);
    delay_ms(25);          // Ждать стабилизации напряжений

    send_config(ST7789_CONFIG, sizeof(ST7789_CONFIG));
    send_command(MADCTL);
    send_byte(position);
    set_rgb_format();
    send_command(SLPOUT);	//	Out of sleep mode
    send_command(NORON);	//	Normal Display on
    send_command(DISPON);	//	Main screen turned on
    release();
  }

protected:
  ATTR_INLINE  void select() { ST_8_CS(CLR); flag = 0; }
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
    send_command(CASET); // Column Address Set
    send_word(x0);
    send_word(x1);

    send_command(RASET); // Row Address Set
    send_word(y0);
    send_word(y1);

    send_command(RAMWR); // Memory Write
  }

  ATTR_INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }

  ATTR_INLINE void send_rgb(C color)
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

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
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

private:
  virtual void send_config(const uint8_t *config, uint8_t size) = 0;
  ATTR_INLINE void set_rgb_format();
  uint8_t flag = 0;
};

template<>
ATTR_INLINE void ST7789<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
ATTR_INLINE void ST7789<RGB12>::send_rgb(RGB12 color)
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

template<>
void ST7789<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);

#ifdef MIK32V2
  uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
  while (len--) send_word(color.rgb);
#else
  uint16_t x = x1 - x0;
  uint16_t y = y1 - y0;
  for (uint16_t i = 0; i <= x; i++)
    for (uint16_t j = 0; j <= y; j++) {
      send_word(color.rgb);
    }
#endif
  release();
}

template<>
void ST7789<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
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

template<>
void ST7789<RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x67); // 8x8x8 bit (24 bit transfer)
}
template<>
void ST7789<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}
template<>
void ST7789<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x55); // 5x6x5 bit
}
template<>
void ST7789<RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x53); // 4x4x4 bit
}
