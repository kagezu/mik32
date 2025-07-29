#pragma once

#ifdef __AVR_ATmega328P__

#include "i2c.h"
#include "comon/include.h"
#include "const.h"
#include "pins.h"

class SSD1306 {
private:
  I2C i2c;
  uint8_t begin_x, begin_y, end_x, end_y, curent_x, curent_y;
  uint8_t buffer[512];

  void send_config(const uint8_t *, uint8_t);

public:
  using RGB = bool; // Только 1 бит
  ATTR_INLINE constexpr int8_t max_x() { return 127; }
  ATTR_INLINE constexpr int8_t max_y() { return 31; }

  ATTR_INLINE void select() {} // { i2c.begin(); }
  ATTR_INLINE void release() { update(); }
  ATTR_INLINE void send_byte(uint8_t data) { i2c.write(data); }
  ATTR_INLINE void send_word(uint16_t data) { i2c.write(data >> 8); i2c.write(data); }
  ATTR_INLINE void send_rgb(bool color, uint8_t len) { while (len--)send_rgb(color); }
  void send_rgb(bool color);

  void pixel(uint8_t x, uint8_t y, bool color);
  void area(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, bool color);

  void init(uint8_t rotation);
  void update();


  void set_addr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

protected:

  void send_command(uint8_t);
  void send_command(uint8_t, uint8_t);
  void send_command(uint8_t, uint8_t, uint8_t);
};

#endif
