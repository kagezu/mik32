#pragma once
#include "pins.h"
#include "type/include.h"

// #define ST_8_WRITE  ST_8_RD(SET); ST_8_PORT(OUT) | 0xFF;
// #define ST_8_READ   ST_8_PORT(IN) & 0x00; ST_8_RD(CLR);

template<typename C = RGB18>
class ST7789 {
public:
  // Разрешение дисплея
  GCC_INLINE constexpr int16_t max_x() { return 239; }
  GCC_INLINE constexpr int16_t max_y() { return 319; }

  void init(uint8_t position)
  {
  #ifdef MIK32V2
    ST_8_RD(GPIO); ST_8_WR(GPIO); ST_8_RS(GPIO); ST_8_CS(GPIO); ST_8_RST(GPIO);
  #endif
    ST_8_RD(OUT); ST_8_WR(OUT); ST_8_RS(OUT); ST_8_CS(OUT); ST_8_RST(OUT);
    ST_8_PORT(OUT) | 0xFF;
    ST_8_RD(SET); ST_8_WR(CLR); ST_8_RS(CLR); ST_8_CS(SET); ST_8_RST(CLR);
    ST_8_RST(SET);

    select();             // CS Выбор дисплея
    send_command(SWRESET);
    delay_ms(15);          // Ждать стабилизации напряжений

    send_config(ST7789_CONFIG, sizeof(ST7789_CONFIG));
    send_command(MADCTL);
    send_byte(position);

    send_command(COLMOD);
    send_byte(0x06); // RGB18

    send_command(SLPOUT);	//	Out of sleep mode
    send_command(NORON);		//	Normal Display on
    send_command(DISPON);	//	Main screen turned on
    release();
  }

protected:
  inline void select() { ST_8_CS(CLR); }
  inline void release() { ST_8_CS(SET); }

  void send_command(uint8_t command)
  {
    ST_8_RS(CLR);
    send_byte(command);
    ST_8_RS(SET);
  }

  void send_byte(uint8_t data)
  {
  #ifdef MIK32V2
    ST_8_PORT(OUTPUT) = data | (ST_8_PORT(OUTPUT) & ~0xff);
    ST_8_WR(CLR); ST_8_WR(SET);
  #else
    ST_8_PORT(OUTPUT) = data;
    ST_8_WR(SET); ST_8_WR(CLR);
  #endif
  }

  void send_word(uint16_t data)
  {
  #ifdef MIK32V2
    volatile reg tmp = (ST_8_PORT(OUTPUT) & ~0xff);
    ST_8_PORT(OUTPUT) = (data >> 8) | tmp;
    ST_8_WR(CLR);
    ST_8_WR(SET);
    ST_8_PORT(OUTPUT) = (data & 0xff) | tmp;
    ST_8_WR(CLR);
    ST_8_WR(SET);
  #else
    ST_8_PORT(OUTPUT) = to_byte(data, 1);
    ST_8_WR(INV); ST_8_WR(INV);
    ST_8_PORT(OUTPUT) = to_byte(data, 0);
    ST_8_WR(INV); ST_8_WR(INV);
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
    static reg tmp = ST_8_PORT(OUTPUT) & ~0xff;
    ST_8_PORT(OUTPUT) = color.red | tmp;
    ST_8_WR(CLR); ST_8_WR(SET);
    ST_8_PORT(OUTPUT) = color.green | tmp;
    ST_8_WR(CLR); ST_8_WR(SET);
    ST_8_PORT(OUTPUT) = color.blue | tmp;
    ST_8_WR(CLR); ST_8_WR(SET);
  #else
    ST_8_PORT(OUTPUT) = color.red;
    ST_8_WR(INV); ST_8_WR(INV);
    ST_8_PORT(OUTPUT) = color.green;
    ST_8_WR(INV); ST_8_WR(INV);
    ST_8_PORT(OUTPUT) = color.blue;
    ST_8_WR(INV); ST_8_WR(INV);
  #endif
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);

  #ifdef MIK32V2
    reg red = (ST_8_PORT(OUTPUT) & ~0xff) | color.red;
    reg green = (ST_8_PORT(OUTPUT) & ~0xff) | color.green;
    reg blue = (ST_8_PORT(OUTPUT) & ~0xff) | color.blue;
    reg red_c = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.red;
    reg green_c = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.green;
    reg blue_c = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.blue;
    reg len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      ST_8_PORT(OUTPUT) = red;
      ST_8_PORT(OUTPUT) = red_c;
      ST_8_WR(SET);
      ST_8_PORT(OUTPUT) = green;
      ST_8_PORT(OUTPUT) = green_c;
      ST_8_WR(SET);
      ST_8_PORT(OUTPUT) = blue;
      ST_8_PORT(OUTPUT) = blue_c;
      ST_8_WR(SET);
    #else
    for (uint16_t i = y0; i <= y1; i++)
      for (uint16_t j = x0; j <= x1; j++) {
        ST_8_PORT(OUTPUT) = color.red;
        ST_8_WR(INV); ST_8_WR(INV);
        ST_8_PORT(OUTPUT) = color.green;
        ST_8_WR(INV); ST_8_WR(INV);
        ST_8_PORT(OUTPUT) = color.blue;
        ST_8_WR(INV); ST_8_WR(INV);
      #endif
    }
    release();
  }

private:
  void send_config(const uint8_t * config, uint8_t size)
    virtual void send_config(const uint8_t * config, uint8_t size) = 0;
};
