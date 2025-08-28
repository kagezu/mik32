#pragma once
#include "comon/include.h"
#include "pins.h"

template <typename C>
class ILI9486_16 : public IDriver {
private:
  uint16_t flag;
  ATTR_INLINE void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 319; }
  ATTR_INLINE constexpr int16_t max_y() { return 479; }
  ATTR_INLINE void select() { L_CS(CLR); }
  ATTR_INLINE void release() { L_CS(SET); }
  ATTR_INLINE void send_rgb(C color, int32_t len) {
    while (len--) send_rgb(color);
  }

  void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0) {
    GPIO_ILI9486_16();

    select();
    send_command(SLPOUT);
    delay_ms(30);
    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL);
    send_byte((rotation | 0x08));  // BGR -> RBG
    set_rgb_format();
    send_command(NORON);
    send_command(DISPON);
    release();
  }

  ATTR_INLINE void send_command(uint8_t command) {
    L_RS(CLR);
    send_byte(command);
    L_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data) {
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    // delay_us(1);
    L_WR(CLR);
  }

  ATTR_INLINE void send_word(uint16_t data) {
    L_PORT(OUTPUT) = (data >> 8);
    L_WR(SET);
    // delay_us(1);
    L_WR(CLR);
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    // delay_us(1);
    L_WR(CLR);
  }

  void pixel(int16_t x, int16_t y, C color) {
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }
};

#include "rgb16.tpp"
#include "rgb18.tpp"
