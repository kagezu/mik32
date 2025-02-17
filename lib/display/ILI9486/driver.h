#pragma once
#include "pins.h"
#include "const/ILI9486.h"
#include "type/include.h"

#define LCD_DRIVER    ILI9486

#define L_WRITE  L_RD(SET); L_PORT(OUT) | 0xFF;
#define L_READ   L_PORT(IN) & 0x00; L_RD(CLR);

template<typename C>
class ILI9486 {
public:
  inline constexpr uint16_t max_x() { return LCD_FLIP & EX_X_Y ? MAX_Y : MAX_X; }
  inline constexpr uint16_t max_y() { return LCD_FLIP & EX_X_Y ? MAX_X : MAX_Y; }

  void init()
  {
    L_RD(OUT); L_WR(OUT); L_RS(OUT); L_CS(OUT); L_RST(OUT);
    L_RD(SET); L_WR(CLR); L_RS(CLR); L_CS(SET); L_RST(CLR);
    L_PORT(OUT) | 0xFF;

    delay_ms(2);
    L_RST(SET);
    delay_ms(15);         // Ждать стабилизации напряжений
    L_CS(CLR);            // CS Выбор дисплея
    send_command(SLPOUT);	//	Out of sleep mode
    delay_ms(10);

    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL); send_byte((LCD_FLIP | 0x08));// BGR -> RBG & ~EX_X_Y

    set_rgb_format();

    send_command(NORON);  //	Normal Display on
    send_command(DISPON);	//	Main screen turned on
    L_CS(SET);
  }

protected:
  void send_command(uint8_t command)
  {
    L_RS(CLR);
    send_byte(command);
    L_RS(SET);
  }

  void send_byte(uint8_t data)
  {
    L_PORT(MMO) = data;
    L_WR(SET); L_WR(CLR);
  }

  void send_word(uint16_t data)
  {
    L_PORT(MMO) = to_byte(data, 1);
    L_WR(INV); L_WR(INV);
    L_PORT(MMO) = to_byte(data, 0);
    L_WR(INV); L_WR(INV);
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
    L_PORT(MMO) = color.red;
    L_WR(INV); L_WR(INV);
    L_PORT(MMO) = color.green;
    L_WR(INV); L_WR(INV);
    L_PORT(MMO) = color.blue;
    L_WR(INV); L_WR(INV);
  }


  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    L_CS(CLR);
    RGB32 rgb = color.rgb32();
    set_addr(x0, y0, x1, y1);
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        L_PORT(MMO) = rgb.red;
        L_WR(INV); L_WR(INV);
        L_PORT(MMO) = rgb.green;
        L_WR(INV); L_WR(INV);
        L_PORT(MMO) = rgb.blue;
        L_WR(INV); L_WR(INV);
      }

    L_CS(SET);
  }

private:
  void set_rgb_format();
  virtual  void send_config(const uint8_t *, uint8_t) = 0;
};

template<>
void ILI9486<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
void ILI9486<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  L_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t x = x1 - x0;
  uint16_t y = y1 - y0;
  for (uint16_t i = 0; i <= x; i++)
    for (uint16_t j = 0; j <= y; j++) {
      send_word(color.rgb);
    }

  L_CS(SET);
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
