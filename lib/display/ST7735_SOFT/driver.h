#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C = RGB16>
class ST7735_SOFT : public IDriver {
private:
  void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 127; }
  ATTR_INLINE constexpr int16_t max_y() { return 159; }
  ATTR_INLINE void select() { ST_SOFT_CS(CLR); }
  ATTR_INLINE void release() { ST_SOFT_SCK(CLR); ST_SOFT_CS(SET); }
  ATTR_INLINE void send_rgb(C color, uint8_t len) { while (len--)send_rgb(color); }

  void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0)
  {
    ST_SOFT_SCK(GPIO); ST_SOFT_SDA(GPIO); ST_SOFT_RST(GPIO); ST_SOFT_CS(GPIO); ST_SOFT_RS(GPIO);
    ST_SOFT_SCK(OUT); ST_SOFT_SDA(OUT); ST_SOFT_RST(OUT); ST_SOFT_CS(OUT); ST_SOFT_RS(OUT);
    ST_SOFT_CS(SET); ST_SOFT_RS(SET);

    ST_SOFT_RST(CLR);               // Аппаратный сброс
    delay_ms(2);
    ST_SOFT_RST(SET);
    delay_ms(15);                // Ждать стабилизации напряжений

    select();
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(rotation);
    set_rgb_format();
    send_command(DISPON);
    release();
  }

  void send_command(uint8_t command)
  {
    ST_SOFT_RS(CLR); // Запись команды
    send_byte(command);
    ST_SOFT_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); send_word(x0); send_word(x1);
    send_command(RASET); send_word(y0); send_word(y1);
    send_command(RAMWR);
  }

  void send_byte(uint8_t data)
  {
    for (uint8_t mask = 0x80; mask; mask >>= 1) {
      if (data & mask) ST_SOFT_SDA(SET);
      else ST_SOFT_SDA(CLR);
      ST_SOFT_SCK(SET);
      ST_SOFT_SCK(CLR);
    }
  }

  void send_word(uint16_t rgb)
  {
  #ifdef __AVR__
    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);

    ST_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
  #else

    int mask = 0x8000;
    while (mask) {
      if (rgb & mask)  ST_SOFT_SDA(SET);
      else  ST_SOFT_SDA(CLR);
      ST_SOFT_SCK(SET);
      ST_SOFT_SCK(CLR);
      mask >>= 1;
    }
  #endif
  }
};

#include "rgb12.tpp"
#include "rgb16.tpp"
#include "rgb18.tpp"
