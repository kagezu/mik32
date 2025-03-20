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
    SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR); // D9 -> PORT 0.3
    L_WR(GPIO); L_RS(GPIO); L_CS(GPIO); // L_RST(GPIO);
    // L_PORT(GPIO) & 0x00;
    PAD_CONFIG->PORT_0_CFG = 0;
    L_WR(OUT); L_RS(OUT); L_CS(OUT); L_PORT(OUT) | 0xFFFF;
    L_WR(CLR); L_RS(CLR); L_CS(SET);

    select();             // CS Выбор дисплея

    // send_command(0x11);		// Sleep OUT
    // delay_ms(50);

    // send_command(0xF2);		// ?????
    // send_byte(0x1C);
    // send_byte(0xA3);
    // send_byte(0x32);
    // send_byte(0x02);
    // send_byte(0xb2);
    // send_byte(0x12);
    // send_byte(0xFF);
    // send_byte(0x12);
    // send_byte(0x00);

    // send_command(0xF1);		// ?????
    // send_byte(0x36);
    // send_byte(0xA4);

    // send_command(0xF8);		// ?????
    // send_byte(0x21);
    // send_byte(0x04);

    // send_command(0xF9);		// ?????
    // send_byte(0x00);
    // send_byte(0x08);

    // send_command(0xC0);		// Power Control 1
    // send_byte(0x0d);
    // send_byte(0x0d);

    // send_command(0xC1);		// Power Control 2
    // send_byte(0x43);
    // send_byte(0x00);

    // send_command(0xC2);		// Power Control 3
    // send_byte(0x00);

    // send_command(0xC5);		// VCOM Control
    // send_byte(0x00);
    // send_byte(0x48);

    // send_command(0xB6);		// Display Function Control
    // send_byte(0x00);
    // send_byte(0x22);		// 0x42 = Rotate display 180 deg.
    // send_byte(0x3B);

    // send_command(0xE0);		// PGAMCTRL (Positive Gamma Control)
    // send_byte(0x0f);
    // send_byte(0x24);
    // send_byte(0x1c);
    // send_byte(0x0a);
    // send_byte(0x0f);
    // send_byte(0x08);
    // send_byte(0x43);
    // send_byte(0x88);
    // send_byte(0x32);
    // send_byte(0x0f);
    // send_byte(0x10);
    // send_byte(0x06);
    // send_byte(0x0f);
    // send_byte(0x07);
    // send_byte(0x00);

    // send_command(0xE1);		// NGAMCTRL (Negative Gamma Control)
    // send_byte(0x0F);
    // send_byte(0x38);
    // send_byte(0x30);
    // send_byte(0x09);
    // send_byte(0x0f);
    // send_byte(0x0f);
    // send_byte(0x4e);
    // send_byte(0x77);
    // send_byte(0x3c);
    // send_byte(0x07);
    // send_byte(0x10);
    // send_byte(0x05);
    // send_byte(0x23);
    // send_byte(0x1b);
    // send_byte(0x00);

    // send_command(0x20);		// Display Inversion OFF
    // send_byte(0x00);//C8 	 

    // send_command(0x36);		// Memory Access Control
    // send_byte(0x0A);

    // send_command(0x3A);		// Interface Pixel Format
    // send_byte(0x55);

    // send_command(0x2A);		// Column Addess Set
    // send_byte(0x00);
    // send_byte(0x00);
    // send_byte(0x01);
    // send_byte(0xDF);

    // send_command(0x002B);		// Page Address Set
    // send_byte(0x00);
    // send_byte(0x00);
    // send_byte(0x01);
    // send_byte(0x3f);
    // delay_ms(50);
    // send_command(0x29);		// Display ON
    // send_command(0x2C);		// Memory Write



    send_command(SLPOUT);	// Out of sleep mode
    // send_command(SWRESET);
    delay_ms(10);

    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL); send_byte((LCD_FLIP | 0x08));// BGR -> RBG & ~EX_X_Y

    set_rgb_format();

    delay_ms(10);
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
    L_PORT(MMO) = data;
    L_WR(CLR); L_WR(SET); L_WR(CLR);
  }

  void send_word(uint16_t data)
  {
    L_PORT(MMO) = data >> 8;
    L_WR(CLR); L_WR(CLR); L_WR(SET); L_WR(CLR);
    L_PORT(MMO) = data;
    L_WR(CLR); L_WR(CLR); L_WR(SET); L_WR(CLR);
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
    L_PORT(MMO) = color.rgb;
    L_WR(CLR); L_WR(CLR); L_WR(SET);
  }


  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);

    //   volatile reg red = (L_PORT(MMO) & ~0xff) | color.red;
    //   volatile reg red_c = (L_PORT(MMO) & ~(0xff | L_WR(MASK))) | color.red;
    reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

    L_PORT(MMO) = color.rgb;
    while (len--) {
      L_WR(CLR); L_WR(CLR); L_WR(SET);
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
  L_PORT(MMO) = color.rgb16();
  L_WR(CLR); L_WR(CLR); L_WR(SET);
}

template<>
void ILI9486<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  reg len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;

  while (len--) {
    L_PORT(MMO) = color.red;
    L_WR(CLR); L_WR(SET);
    L_PORT(MMO) = color.green;
    L_WR(CLR); L_WR(SET);
    L_PORT(MMO) = color.blue;
    L_WR(CLR); L_WR(SET);
    L_PORT(MMO) = color.red;
    L_WR(CLR); L_WR(SET);
    L_PORT(MMO) = color.green;
    L_WR(CLR); L_WR(SET);
    L_PORT(MMO) = color.blue;
    L_WR(CLR); L_WR(SET);
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
