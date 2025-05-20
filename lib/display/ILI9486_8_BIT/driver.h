#pragma once
#include "pins.h"
#include "const.h"
#include "type/include.h"

#define L_BEGIN       L_CS(CLR);
#define L_END         L_CS(SET);

#define L_WRITE  L_RD(SET); L_PORT(OUT) | 0xFF;
#define L_READ   L_PORT(IN) & 0x00; L_RD(CLR);

template<typename C>
class ILI9486_8 {
public:
  inline constexpr uint16_t max_x() { return LCD_FLIP & EX_X_Y ? MAX_Y : MAX_X; }
  inline constexpr uint16_t max_y() { return LCD_FLIP & EX_X_Y ? MAX_X : MAX_Y; }

  void init()
  {
  #ifdef MIK32V2
    L_RD(GPIO); L_WR(GPIO); L_RS(GPIO); L_CS(GPIO); L_RST(GPIO);
  #endif
    L_RD(OUT); L_WR(OUT); L_RS(OUT); L_CS(OUT); L_RST(OUT);
    L_PORT(OUT) | 0xFF;
    L_RD(SET); L_WR(CLR); L_RS(CLR); L_CS(SET); L_RST(CLR);
    L_RST(SET);

    select();             // CS Выбор дисплея
    send_command(SLPOUT);	// Out of sleep mode
    delay_ms(50);

    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL); send_byte((LCD_FLIP | 0x08));// BGR -> RBG & ~EX_X_Y

    set_rgb_format();

    delay_ms(50);
    send_command(NORON);  // Normal Display on
    send_command(DISPON);	// Main screen turned on
    release();
  }

  void scroll(uint16_t sl)
  {
    select();            // CS Выбор дисплея
    send_command(VSCRSADD);
    send_word(sl);
    release();
  }

protected:
  inline void select() { L_CS(CLR); }
  inline void release() { L_CS(SET); }

  void send_command(uint8_t command)
  {
    L_RS(CLR);
    send_byte(command);
    L_RS(SET);
  }

  void send_byte(uint8_t data)
  {
  #ifdef MIK32V2
    L_PORT(OUTPUT) = data | (L_PORT(OUTPUT) & ~0xff);
    L_WR(CLR); L_WR(SET);
  #else
    L_PORT(OUTPUT) = data;
    L_WR(SET); L_WR(CLR);
  #endif
  }

  void send_word(uint16_t data)
  {
  #ifdef MIK32V2
    static volatile reg tmp = L_PORT(OUTPUT) & ~(0xff | L_WR(MASK));
    L_PORT(OUTPUT) = (data >> 8) | tmp;
    L_WR(SET);
    L_PORT(OUTPUT) = (data & 0xff) | tmp;
    L_WR(SET);
  #else
    L_PORT(OUTPUT) = to_byte(data, 1);
    L_WR(INV); L_WR(INV);
    L_PORT(OUTPUT) = to_byte(data, 0);
    L_WR(INV); L_WR(INV);
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

  void send_rgb(C color)
  {
  #ifdef MIK32V2
    static volatile reg mask = L_PORT(OUTPUT) & ~(0xff | L_WR(MASK));
    L_PORT(OUTPUT) = color.red | mask;
    L_WR(SET);
    L_PORT(OUTPUT) = color.green | mask;
    L_WR(SET);
    L_PORT(OUTPUT) = color.blue | mask;
    L_WR(SET);
  #else
    L_PORT(OUTPUT) = color.red;
    L_WR(INV); L_WR(INV);
    L_PORT(OUTPUT) = color.green;
    L_WR(INV); L_WR(INV);
    L_PORT(OUTPUT) = color.blue;
    L_WR(INV); L_WR(INV);
  #endif
  }


  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    volatile reg red = (L_PORT(OUTPUT) & ~0xff) | color.red;
    volatile reg green = (L_PORT(OUTPUT) & ~0xff) | color.green;
    volatile reg blue = (L_PORT(OUTPUT) & ~0xff) | color.blue;
    volatile reg red_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | color.red;
    volatile reg green_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | color.green;
    volatile reg blue_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | color.blue;
    reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      L_PORT(OUTPUT) = red_c;
      L_PORT(OUTPUT) = red;
      L_PORT(OUTPUT) = green_c;
      L_PORT(OUTPUT) = green;
      L_PORT(OUTPUT) = blue_c;
      L_PORT(OUTPUT) = blue;
    #else
    RGB32 rgb = color.rgb32();
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        L_PORT(OUTPUT) = rgb.red;
        L_WR(INV); L_WR(INV);
        L_PORT(OUTPUT) = rgb.green;
        L_WR(INV); L_WR(INV);
        L_PORT(OUTPUT) = rgb.blue;
        L_WR(INV); L_WR(INV);
      #endif
    }

    release();
  }

private:
  void set_rgb_format();
  virtual void send_config(const uint8_t * config, uint8_t size) = 0;
};

template<>
void ILI9486_8<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
void ILI9486_8<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);

#ifdef MIK32V2
  volatile  reg h = (L_PORT(OUTPUT) & ~0xff) | (color.rgb >> 8);
  volatile  reg l = (L_PORT(OUTPUT) & ~0xff) | (color.rgb & 0xff);
  volatile  reg h_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | (color.rgb >> 8);
  volatile  reg l_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | (color.rgb & 0xff);
  reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) {
    L_PORT(OUTPUT) = h_c;
    L_PORT(OUTPUT) = h;
    L_PORT(OUTPUT) = l_c;
    L_PORT(OUTPUT) = l;
  }
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
void ILI9486_8<RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}
template<>
void ILI9486_8<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
}
template<>
void ILI9486_8<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
}
