#pragma once
#include "const.h"
#include "pin.h"
#include "type/rgb.h"

class ST7735_SOFT {
protected:
  void send_command(int32_t data);
  void set_addr(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
  void send_zero();
  void send_byte(int32_t data);
  void send_rgb(RGB color);
  void rect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, RGB color);
  // void send_rgb(uint32_t16_t data);
};

void ST7735_SOFT::send_command(int32_t command)
{
  L_RS(RES); // Запись команды
  send_byte(command);
  L_RS(SET); // Запись данных
};

void ST7735_SOFT::set_addr(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  send_command(CASET); // Column Address Set
  send_zero();
  send_byte(x0);
  send_zero();
  send_byte(x1);

  send_command(RASET); // Row Address Set
  send_zero();
  send_byte(y0);
  send_zero();
  send_byte(y1);

  send_command(RAMWR); // Memory Write
};

void ST7735_SOFT::send_zero()
{
  L_SDA(RES);

  int32_t res = L_SCK(SFR) & ~L_SCK(MASK);
  int32_t set = L_SCK(SFR) | L_SCK(MASK);

  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
};

void ST7735_SOFT::send_byte(int32_t data)
{
  int32_t b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  int32_t b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  int32_t set = L_SCK(SFR) | L_SCK(MASK);

  L_SCK(SFR) = data & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x2 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x1 ? b1 : b0;
  L_SCK(SFR) = set;
};


void ST7735_SOFT::send_rgb(RGB color)
{
  int32_t r = color.red;
  int32_t g = color.green;
  int32_t b = color.blue;

  int32_t b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  int32_t b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  int32_t set = L_SCK(SFR) | L_SCK(MASK);

  L_SCK(SFR) = r & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = r & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = r & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = r & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = r & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = r & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = g & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = g & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = b & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = b & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = b & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
#endif
};

void ST7735_SOFT::rect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, RGB color)
{
  int32_t r = color.red;
  int32_t g = color.green;
  int32_t b = color.blue;

  L_CS(RES);
  set_addr(x0, y0, x1, y1);
  int32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  int32_t b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  int32_t b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  int32_t set = L_SCK(SFR) | L_SCK(MASK);

  // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
  while (len--) {
    L_SCK(SFR) = r & 0x80 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = r & 0x40 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = r & 0x20 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = r & 0x10 ? b1 : b0;
    L_SCK(SFR) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(SFR) = r & 0x8 ? b1 : b0;
    L_SCK(SFR) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(SFR) = r & 0x4 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
  #endif

    L_SCK(SFR) = g & 0x80 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x40 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x20 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x10 ? b1 : b0;
    L_SCK(SFR) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(SFR) = g & 0x8 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x4 ? b1 : b0;
    L_SCK(SFR) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
  #endif

    L_SCK(SFR) = b & 0x80 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b & 0x40 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b & 0x20 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b & 0x10 ? b1 : b0;
    L_SCK(SFR) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(SFR) = b & 0x8 ? b1 : b0;
    L_SCK(SFR) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(SFR) = b & 0x4 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
  #endif
  }
  L_CS(SET);
};
