#pragma once
#include "pins.h"
#include "ILI9225/const.h"
#include "comon/include.h"

class ILI9225_SOFT {
private:
  uint8_t R;
  void send_config(const uint16_t *config, uint8_t size);

public:
  using RGB = RGB16; // Только 16 бит
  void init(uint8_t rotation = 0);
  ATTR_INLINE constexpr int16_t max_x() { return 175; }
  ATTR_INLINE constexpr int16_t max_y() { return 219; }
  ATTR_INLINE void select() { ILI_SOFT_CS(CLR); }
  ATTR_INLINE void release() { ILI_SOFT_SCK(CLR); ILI_SOFT_CS(SET); }
  ATTR_INLINE void send_rgb(RGB16 color, uint8_t len) { while (len--)send_rgb(color); }
  ATTR_INLINE void send_rgb(RGB16 color) { send_word(color.rgb); }

  void send_command(uint16_t command);
  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void send_word(uint16_t rgb);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color);
};
