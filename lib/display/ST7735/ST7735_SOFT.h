#pragma once
#include "const.h"
#include "pin.h"
#include "type/rgb.h"

class ST7735_SOFT {
protected:
  void send_command(uint8_t data);
  void set_addr(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
  void send_zero();
  void send_byte(uint8_t data);
  void send_rgb(RGB color);
  void rect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, RGB color);
};

// void ST7735_SOFT::send_command(uint8_t command)
// {
//   L_RS(RES); // Запись команды
//   send_byte(command);
//   L_RS(SET); // Запись данных
// };

void ST7735_SOFT::set_addr(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
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

  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);

};

void ST7735_SOFT::send_byte(uint8_t data)
{
  uint32_t d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint32_t d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint32_t s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
  uint32_t s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);


  if (data & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x2) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x1) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  L_SCK(RES);

};


void ST7735_SOFT::send_rgb(RGB color)
{
  uint8_t r = color.red;
  uint8_t g = color.green;
  uint8_t b = color.blue;

  uint32_t d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint32_t d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint32_t s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
  uint32_t s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

  if (r & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (r & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (r & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (r & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16

  if (r & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

#endif
#if RGB_FORMAT == RGB_18

  if (r & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  L_SCK(MMO) = d0;
  L_SCK(MMO) = s0;
  L_SCK(MMO) = d0;
  L_SCK(MMO) = s0;

#endif

  if (g & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (g & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (g & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (g & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16

  if (g & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (g & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

#endif
#if RGB_FORMAT == RGB_18

  L_SCK(MMO) = d0;
  L_SCK(MMO) = s0;
  L_SCK(MMO) = d0;
  L_SCK(MMO) = s0;

#endif

  if (b & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (b & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (b & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (b & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16

  if (b & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

#endif
#if RGB_FORMAT == RGB_18

  if (b & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  L_SCK(MMO) = d0;
  L_SCK(MMO) = s0;
  L_SCK(MMO) = d0;
  L_SCK(MMO) = s0;

#endif

  L_SCK(RES);
};

void ST7735_SOFT::rect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, RGB color)
{
  uint8_t r = color.red;
  uint8_t g = color.green;
  uint8_t b = color.blue;

  L_CS(RES);
  set_addr(x0, y0, x1, y1);
  uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  uint32_t d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint32_t d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint32_t s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
  uint32_t s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

  // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение до 2х раз
  while (len--) {
    if (r & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16

    if (r & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  #endif
  #if RGB_FORMAT == RGB_18

    if (r & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

  #endif

    if (g & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16

    if (g & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  #endif
  #if RGB_FORMAT == RGB_18

    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

  #endif

    if (b & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16

    if (b & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  #endif
  #if RGB_FORMAT == RGB_18

    if (b & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

  #endif
  }
  L_SCK(RES);
  L_CS(SET);
};
