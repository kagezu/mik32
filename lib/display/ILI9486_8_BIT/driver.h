#pragma once
#include "pins.h"
#include "type/include.h"

// #define ILI_8_BEGIN       ILI_8_CS(CLR);
// #define ILI_8_END         ILI_8_CS(SET);

// #define ILI_8_WRITE  ILI_8_RD(SET); ILI_8_PORT(OUT) | 0xFF;
// #define ILI_8_READ   ILI_8_PORT(IN) & 0x00; ILI_8_RD(CLR);

template<typename C = RGB32>
class ILI9486_8 {
public:
  // Разрешение дисплея
  ATTR_INLINE constexpr int16_t max_x() { return 319; }
  ATTR_INLINE constexpr int16_t max_y() { return 479; }

  void init(uint8_t position)
  {
  #ifdef MIK32V2
    ILI_8_RD(GPIO); ILI_8_WR(GPIO); ILI_8_RS(GPIO); ILI_8_CS(GPIO); ILI_8_RST(GPIO);
  #endif
    ILI_8_RD(OUT); ILI_8_WR(OUT); ILI_8_RS(OUT); ILI_8_CS(OUT); ILI_8_RST(OUT);
    ILI_8_PORT(OUT) | 0xFF;
    ILI_8_RD(SET); ILI_8_WR(CLR); ILI_8_RS(CLR); ILI_8_CS(SET); ILI_8_RST(CLR);
    ILI_8_RST(SET);

    select();             // CS Выбор дисплея
    send_command(SLPOUT);	// Out of sleep mode
    delay_ms(50);

    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL); send_byte((position | 0x08));// BGR -> RBG & ~EX_X_Y

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
  inline void select() { ILI_8_CS(CLR); }
  inline void release() { ILI_8_CS(SET); }

  void send_command(uint8_t command)
  {
    ILI_8_RS(CLR);
    send_byte(command);
    ILI_8_RS(SET);
  }

  void send_byte(uint8_t data)
  {
  #ifdef MIK32V2
    ILI_8_PORT(OUTPUT) = data | (ILI_8_PORT(OUTPUT) & ~0xff);
    ILI_8_WR(CLR); ILI_8_WR(SET);
  #else
    ILI_8_PORT(OUTPUT) = data;
    ILI_8_WR(SET); ILI_8_WR(CLR);
  #endif
  }

  void send_word(uint16_t data)
  {
  #ifdef MIK32V2
    static volatile reg tmp = ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK));
    ILI_8_PORT(OUTPUT) = (data >> 8) | tmp;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = (data & 0xff) | tmp;
    ILI_8_WR(SET);
  #else
    ILI_8_PORT(OUTPUT) = to_byte(data, 1);
    ILI_8_WR(INV); ILI_8_WR(INV);
    ILI_8_PORT(OUTPUT) = to_byte(data, 0);
    ILI_8_WR(INV); ILI_8_WR(INV);
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
    static volatile reg mask = ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK));
    ILI_8_PORT(OUTPUT) = color.red | mask;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = color.green | mask;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = color.blue | mask;
    ILI_8_WR(SET);
  #else
    ILI_8_PORT(OUTPUT) = color.red;
    ILI_8_WR(INV); ILI_8_WR(INV);
    ILI_8_PORT(OUTPUT) = color.green;
    ILI_8_WR(INV); ILI_8_WR(INV);
    ILI_8_PORT(OUTPUT) = color.blue;
    ILI_8_WR(INV); ILI_8_WR(INV);
  #endif
  }


  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    volatile reg red = (ILI_8_PORT(OUTPUT) & ~0xff) | color.red;
    volatile reg green = (ILI_8_PORT(OUTPUT) & ~0xff) | color.green;
    volatile reg blue = (ILI_8_PORT(OUTPUT) & ~0xff) | color.blue;
    volatile reg red_c = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.red;
    volatile reg green_c = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.green;
    volatile reg blue_c = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.blue;
    reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      ILI_8_PORT(OUTPUT) = red_c;
      ILI_8_PORT(OUTPUT) = red;
      ILI_8_PORT(OUTPUT) = green_c;
      ILI_8_PORT(OUTPUT) = green;
      ILI_8_PORT(OUTPUT) = blue_c;
      ILI_8_PORT(OUTPUT) = blue;
    #else
    RGB32 rgb = color.rgb32();
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        ILI_8_PORT(OUTPUT) = rgb.red;
        ILI_8_WR(INV); ILI_8_WR(INV);
        ILI_8_PORT(OUTPUT) = rgb.green;
        ILI_8_WR(INV); ILI_8_WR(INV);
        ILI_8_PORT(OUTPUT) = rgb.blue;
        ILI_8_WR(INV); ILI_8_WR(INV);
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
  volatile  reg h = (ILI_8_PORT(OUTPUT) & ~0xff) | (color.rgb >> 8);
  volatile  reg l = (ILI_8_PORT(OUTPUT) & ~0xff) | (color.rgb & 0xff);
  volatile  reg h_c = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | (color.rgb >> 8);
  volatile  reg l_c = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | (color.rgb & 0xff);
  reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) {
    ILI_8_PORT(OUTPUT) = h_c;
    ILI_8_PORT(OUTPUT) = h;
    ILI_8_PORT(OUTPUT) = l_c;
    ILI_8_PORT(OUTPUT) = l;
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
void ILI9486_8<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
}
