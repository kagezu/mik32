#pragma once
#include "type/include.h"
#include "pins.h"

template<typename C = RGB16>
class ILI9486_16 {
public:
  // Разрешение дисплея
  ATTR_INLINE constexpr int16_t max_x() { return 319; }
  ATTR_INLINE constexpr int16_t max_y() { return 479; }

  void init(uint8_t position = 0)
  {
    // SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR);  // PORT 0.3 -> D9
    L_WR(GPIO); L_RS(GPIO); L_CS(GPIO);
    L_WR(OUT); L_RS(OUT); L_CS(OUT);
    L_WR(CLR); L_RS(CLR); L_CS(SET);
    PAD_CONFIG->PORT_0_CFG = 0;           // PORT 0 -> GPIO
    L_PORT(OUT) | 0xFFFF;                 // PORT 0 -> OUT

    select();                             // CS Выбор дисплея
    send_command(SLPOUT);	                // Out of sleep mode
    delay_ms(50);

    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL);
    send_byte((position | 0x08));         // BGR -> RBG & ~EX_X_Y

    set_rgb_format();

    delay_ms(50);
    send_command(NORON);                  // Normal Display on
    send_command(DISPON);	                // Main screen turned on
    release();
  }

protected:
  ATTR_INLINE  void select() { L_CS(CLR); flag = 0; }
  ATTR_INLINE  void release() { L_CS(SET); }

  ATTR_INLINE void send_command(uint8_t command)
  {
    L_RS(CLR);
    send_byte(command);
    L_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data)
  {
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    L_WR(CLR);
  }

  ATTR_INLINE void send_word(uint16_t data)
  {
    L_PORT(OUTPUT) = (data >> 8);
    L_WR(SET);
    L_WR(CLR);
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    L_WR(CLR);
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

  ATTR_INLINE void send_rgb(C color)
  {
    L_PORT(OUTPUT) = color.rgb;
    L_WR(SET);
    L_WR(CLR);
  }

  ATTR_INLINE void send_rgb(C color, int32_t len)
  {
    L_PORT(OUTPUT) = color.rgb;
    while (len--) {
      L_WR(SET);
      L_WR(CLR);
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);

    uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
    L_PORT(OUTPUT) = color.rgb;
    while (len--) {
      L_WR(SET);
      L_WR(CLR);
    }
    release();
  }

private:
  uint16_t flag;
  ATTR_INLINE void set_rgb_format();
  virtual void send_config(const uint8_t *config, uint8_t size) = 0;
};

template<>
void ILI9486_16<RGB32>::send_rgb(RGB32 color)
{
  static uint16_t half;
  if (flag) {
    L_PORT(OUTPUT) = color.red | half;
    L_WR(SET); L_WR(CLR);
    flag = 0;
    L_PORT(OUTPUT) = color.rgb;
    L_WR(SET); L_WR(CLR);
  }
  else {
    L_PORT(OUTPUT) = color.rgb >> 8;
    L_WR(SET); L_WR(CLR);
    half = color.blue << 8;
    flag = 1;
  }
}

template<>
void ILI9486_16<RGB32>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB32 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint32_t len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;
  uint16_t hword = *(uint16_t *)&color.green;
  uint16_t mword = color.red | (color.blue << 8);
  uint16_t lword = *(uint16_t *)&color.blue;

  while (len--) {
    L_PORT(OUTPUT) = hword;
    L_WR(SET);
    L_WR(CLR);

    L_PORT(OUTPUT) = mword;
    L_WR(SET);
    L_WR(CLR);

    L_PORT(OUTPUT) = lword;
    L_WR(SET);
    L_WR(CLR);
  }
  release();
}

template<>
void ILI9486_16<RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}
template<>
void ILI9486_16<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}
template<>
void ILI9486_16<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x65); // 5x6x5 bit
}
