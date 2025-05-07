#pragma once
#include "pins.h"
#include "const.h"
#include "type/include.h"

#define LCD_DRIVER    ILI9486

template<typename C>
class ILI9486 {
public:
  inline constexpr uint16_t max_x() { return LCD_FLIP & EX_X_Y ? MAX_Y : MAX_X; }
  inline constexpr uint16_t max_y() { return LCD_FLIP & EX_X_Y ? MAX_X : MAX_Y; }

  void init()
  {
    SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR);  // PORT 0.3 -> D9
    L_WR(GPIO); L_RS(GPIO); L_CS(GPIO);
    L_WR(OUT); L_RS(OUT); L_CS(OUT);
    L_WR(CLR); L_RS(CLR); L_CS(SET);
    PAD_CONFIG->PORT_0_CFG = 0;           // PORT 0 -> GPIO
    L_PORT(OUT) | 0xFFFF;                 // PORT 0 -> OUT

    select();             // CS Выбор дисплея
    send_command(SLPOUT);	// Out of sleep mode
    delay_ms(50);

    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL);
    send_byte((LCD_FLIP | 0x08));// BGR -> RBG & ~EX_X_Y

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
    send_word(sl);// < MAX_Y + 1 ? sl : sl % (MAX_Y + 1));
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
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    L_WR(CLR);
  }

  void send_word(uint16_t data)
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

  void send_rgb(C color)
  {
    static reg set = L_WR(OUTPUT) | L_WR(MASK);
    static reg clr = L_WR(OUTPUT) & ~L_WR(MASK);
    L_PORT(OUTPUT) = color.rgb;
    L_WR(OUTPUT) = set;
    L_WR(OUTPUT) = clr;
  }


  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);

    reg len = (x1 - x0 + 1) * (y1 - y0 + 1);
    reg set = L_WR(OUTPUT) | L_WR(MASK);
    reg clr = L_WR(OUTPUT) & ~L_WR(MASK);
    L_PORT(OUTPUT) = color.rgb;
    while (len--) {
      L_WR(OUTPUT) = set;
      L_WR(OUTPUT) = clr;
    }
    release();
  }

private:
  void set_rgb_format();
  virtual  void send_config(const uint8_t *, uint8_t) = 0;
};

template<>
void ILI9486<RGB18>::send_rgb(RGB18 color)
{
  static uint16_t half, flag = 0;

  if (flag) {
    L_PORT(OUTPUT) = color.red | half;
    L_WR(SET); L_WR(CLR);
    flag = 0;
    L_PORT(OUTPUT) = *(uint16_t *)&color.blue;
    L_WR(SET); L_WR(CLR);
  }
  else {
    L_PORT(OUTPUT) = *(uint16_t *)&color.green;
    L_WR(SET); L_WR(CLR);
    half = color.blue << 8;
    flag = 1;
  }
}

template<>
void ILI9486<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  reg len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;
  reg set = L_WR(OUTPUT) | L_WR(MASK);
  reg clr = L_WR(OUTPUT) & ~L_WR(MASK);
  uint16_t hword = *(uint16_t *)&color.green;
  uint16_t mword = color.red | (color.blue << 8);
  uint16_t lword = *(uint16_t *)&color.blue;

  while (len--) {
    L_PORT(OUTPUT) = hword;
    L_WR(OUTPUT) = set;
    L_WR(OUTPUT) = clr;

    L_PORT(OUTPUT) = mword;
    L_WR(OUTPUT) = set;
    L_WR(OUTPUT) = clr;

    L_PORT(OUTPUT) = lword;
    L_WR(OUTPUT) = set;
    L_WR(OUTPUT) = clr;
  }
  release();
}

template<>
void ILI9486<RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}
template<>
void ILI9486<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
}
template<>
void ILI9486<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
}
