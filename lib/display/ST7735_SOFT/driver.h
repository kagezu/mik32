#pragma once
#include "pins.h"
#include "comon/include.h"

template<typename C>
class ST7735_SOFT : public IDriver {};

////////////////////////////////// RGB12 //////////////////////////////////////

template<>
class ST7735_SOFT<RGB12> : public IDriver {

public:
  using RGB = RGB12;

#include "base.h"

  INLINE void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x03); // 4x4x4 bit
  }

  void send_rgb(RGB12 color)
  {
    uint16_t rgb = color.rgb;

    for (uint16_t mask = 0x800; mask; mask >>= 1) {
      if (rgb & mask) ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    uint16_t rgb = color.rgb;

    ST_SOFT_CS.clr();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      for (int mask = 0x800; mask; mask >>= 1) {
        if (rgb & mask) ST_SOFT_SDA.set();
        else ST_SOFT_SDA.clr();
        ST_SOFT_SCK.set();
        ST_SOFT_SCK.clr();
      }
    }

    release();
  }
};

////////////////////////////////// RGB16 //////////////////////////////////////

template<>
class ST7735_SOFT<RGB16> : public IDriver {

public:
  using RGB = RGB16;

#include "base.h"

  void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x05); // 5x6x5 bit
  }

  INLINE void send_rgb(RGB16 color)
  {
    send_word(color.rgb);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    uint16_t rgb = color.rgb;

    ST_SOFT_CS.clr();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      int mask = 0x8000;
      while (mask) {
        if (rgb & mask)  ST_SOFT_SDA.set();
        else  ST_SOFT_SDA.clr();
        ST_SOFT_SCK.set();
        ST_SOFT_SCK.clr();
        mask >>= 1;
      }
    }

    ST_SOFT_SCK.clr();
    ST_SOFT_CS.set();
  }
};

////////////////////////////////// RGB18 //////////////////////////////////////

template<>
class ST7735_SOFT<RGB18> : public IDriver {

public:
  using RGB = RGB18;

#include "base.h"

  void set_rgb_format()
  {
    send_command(COLMOD);
    send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  }

  void send_rgb(RGB18 color)
  {
    uint8_t r = color.red;
    uint8_t g = color.green;
    uint8_t b = color.blue;

    if (r & 0x80)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (r & 0x40)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (r & 0x20)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (r & 0x10)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (r & 0x8)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (r & 0x4)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();

    if (g & 0x80)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (g & 0x40)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (g & 0x20)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (g & 0x10)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (g & 0x8)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (g & 0x4)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();

    if (b & 0x80)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (b & 0x40)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (b & 0x20)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (b & 0x10)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (b & 0x8)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    if (b & 0x4)  ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
  {
    uint8_t r = color.red;
    uint8_t g = color.green;
    uint8_t b = color.blue;

    ST_SOFT_CS.clr();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
    while (len--) {
      if (r & 0x80)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (r & 0x40)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (r & 0x20)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (r & 0x10)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (r & 0x8)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (r & 0x4)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();

      if (g & 0x80)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (g & 0x40)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (g & 0x20)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (g & 0x10)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (g & 0x8)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (g & 0x4)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();

      if (b & 0x80)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (b & 0x40)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (b & 0x20)  ST_SOFT_SDA.set();
      else ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (b & 0x10)  ST_SOFT_SDA.set();
      else  ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (b & 0x8)  ST_SOFT_SDA.set();
      else  ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      if (b & 0x4)  ST_SOFT_SDA.set();
      else  ST_SOFT_SDA.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
      ST_SOFT_SCK.set();
      ST_SOFT_SCK.clr();
    }
    ST_SOFT_CS.set();
  }
};
