#pragma once
#include "pins.h"
#include "const.h"
#include "type/include.h"

#define LCD_DRIVER    ST7789
#define L_BEGIN       L_CS(CLR);
#define L_END         L_CS(SET);

#define L_WRITE  L_RD(SET); L_PORT(OUT) | 0xFF;
#define L_READ   L_PORT(IN) & 0x00; L_RD(CLR);

template<typename C>
class ST7789 {
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
    send_command(SWRESET);
    delay_ms(15);          // Ждать стабилизации напряжений

    send_config(ST7789_CONFIG, sizeof(ST7789_CONFIG));
    send_command(MADCTL); send_byte(LCD_FLIP);

    send_command(COLMOD);
    send_byte(0x06); // RGB18

    send_command(SLPOUT);	//	Out of sleep mode
    send_command(NORON);		//	Normal Display on
    send_command(DISPON);	//	Main screen turned on
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
    volatile reg tmp = (L_PORT(OUTPUT) & ~0xff);
    L_PORT(OUTPUT) = (data >> 8) | tmp;
    L_WR(CLR);
    L_WR(SET);
    L_PORT(OUTPUT) = (data & 0xff) | tmp;
    L_WR(CLR);
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
    static reg tmp = L_PORT(OUTPUT) & ~0xff;
    L_PORT(OUTPUT) = color.red | tmp;
    L_WR(CLR); L_WR(SET);
    L_PORT(OUTPUT) = color.green | tmp;
    L_WR(CLR); L_WR(SET);
    L_PORT(OUTPUT) = color.blue | tmp;
    L_WR(CLR); L_WR(SET);
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
    reg red = (L_PORT(OUTPUT) & ~0xff) | color.red;
    reg green = (L_PORT(OUTPUT) & ~0xff) | color.green;
    reg blue = (L_PORT(OUTPUT) & ~0xff) | color.blue;
    reg red_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | color.red;
    reg green_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | color.green;
    reg blue_c = (L_PORT(OUTPUT) & ~(0xff | L_WR(MASK))) | color.blue;
    reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      L_PORT(OUTPUT) = red;
      L_PORT(OUTPUT) = red_c;
      L_WR(SET);
      L_PORT(OUTPUT) = green;
      L_PORT(OUTPUT) = green_c;
      L_WR(SET);
      L_PORT(OUTPUT) = blue;
      L_PORT(OUTPUT) = blue_c;
      L_WR(SET);
    #else
    for (uint16_t i = y0; i <= y1; i++)
      for (uint16_t j = x0; j <= x1; j++) {
        L_PORT(OUTPUT) = color.red;
        L_WR(INV); L_WR(INV);
        L_PORT(OUTPUT) = color.green;
        L_WR(INV); L_WR(INV);
        L_PORT(OUTPUT) = color.blue;
        L_WR(INV); L_WR(INV);
      #endif
    }
    release();
  }

private:
  void send_config(const uint8_t * config, uint8_t size)
  {
    while (size) {
      uint8_t count = pgm_read_byte(config++);
      uint8_t comand = pgm_read_byte(config++);
      size -= 2 + count;
      send_command(comand);
      while (count--) send_byte(pgm_read_byte(config++));
    }
  }
};
