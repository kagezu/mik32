#pragma once
#include "pins.h"
#include "const/ST7789.h"
#include "type/include.h"

#define LCD_DRIVER    ST7789

#define L_WRITE  L_RD(SET); DDR(L_DATA) = 0xFC;
#define L_READ   DDR(L_DATA) = 0x00; L_RD(CLR);



template<typename C>
class ST7789 {
public:
  inline const uint16_t max_x() { return MAX_X; }
  inline const uint16_t max_y() { return MAX_Y; }

  void init()
  {
    L_RD(OUT); L_WR(OUT); L_RS(OUT); L_CS(OUT); L_RST(OUT);
    L_RD(SET); L_WR(CLR); L_RS(CLR); L_CS(SET); L_RST(CLR);
    // DDR(L_DATA) = 0xFC; //временно
    // DDR(B) |= 0x03; //временно

    delay_us(2000);
    L_RST(SET);
    delay_us(15000);          // Ждать стабилизации напряжений
    L_CS(CLR);                // CS Выбор дисплея

    send_config(ST7789_CONFIG, sizeof(ST7789_CONFIG));
    send_command(MADCTL);
    send_byte(LCD_FLIP);
    set_rgb_format();
    send_command(SLPOUT);	//	Out of sleep mode
    send_command(NORON);		//	Normal Display on
    send_command(DISPON);	//	Main screen turned on	
    delay_ms(50);
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
    // PORT(B) = data & 0x03;
    L_WR(SET); L_WR(CLR);
  }

  void send_word(uint16_t data)
  {
    L_PORT(MMO) = to_byte(data, 1);
    // PORT(B) = to_byte(data, 1) & 0x03;
    L_WR(INV); L_WR(INV);
    L_PORT(MMO) = to_byte(data, 0);
    // PORT(B) = to_byte(data, 0) & 0x03;
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
    set_addr(x0, y0, x1, y1);
    uint16_t x = x1 - x0;
    uint16_t y = y1 - y0;
    for (uint16_t i = 0; i <= x; i++)
      for (uint16_t j = 0; j <= y; j++) {
        L_PORT(MMO) = color.red;
        L_WR(INV); L_WR(INV);
        L_PORT(MMO) = color.green;
        L_WR(INV); L_WR(INV);
        L_PORT(MMO) = color.blue;
        L_WR(INV); L_WR(INV);
      }

    L_CS(SET);
  }

private:
  void set_rgb_format();
  virtual  void send_config(const uint8_t *, uint8_t) = 0;
};

template<>
void ST7789<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
inline void ST7789<RGB12>::send_rgb(RGB12 color)
{
  static uint8_t half, flag = 0;

  if (flag) {
    send_byte(to_byte(color.rgb, 1) | half);
    send_byte(color.rgb);
    flag = 0;
  }
  else {
    send_byte(color.rgb >> 4);
    half = color.rgb << 4;
    flag = 1;
  }
}

template<>
void ST7789<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  L_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;

  uint8_t hbyte = color.rgb >> 4;
  uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
  uint8_t lbyte = color.rgb;

  while (len--) {
    send_byte(hbyte);
    send_byte(mbyte);
    send_byte(lbyte);
  }
  L_CS(SET);
}

template<>
void ST7789<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
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
void ST7789<RGB32>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(_RGB18);
}
template<>
void ST7789<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(_RGB18);
}
template<>
void ST7789<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(_RGB16);
}
template<>
void ST7789<RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(_RGB12);
}
