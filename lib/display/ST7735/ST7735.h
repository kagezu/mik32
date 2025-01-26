#pragma once
#include "ST7735_SOFT.h"

class ST7735 : public ST7735_SOFT {
public:
  ST7735();

public:
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void clear() { rect(0, 0, MAX_X, MAX_Y, _background); }
  void clear(RGB color) { rect(0, 0, MAX_X, MAX_Y, color); }
  void rect_fill(int32_t x, int32_t y, int32_t x1, int32_t y1) { rect(x, y, x1, y1, _color); }

  void symbol(uint8_t *, uint16_t, uint16_t, int32_t, int32_t);
  void pixel(int32_t, int32_t);

  // Скринсейвер
  void demo(int32_t);
  void test(int32_t);

private:
  RGB _color = 0x00ffffff;
  RGB _background = 0;

  void send_config(const uint8_t *, int32_t);
};

void ST7735::send_config(const uint8_t *config, int32_t size)
{
  while (size) {
    int32_t data, comand = *config++;
    size -= 2;
    send_command(comand);
    while ((data = *config++) != 0xFF) {
      send_byte(data);
      size--;
    }
  }
}

ST7735::ST7735()
{
  L_SCK(GPIO);
  L_SDA(GPIO);
  L_RST(GPIO);
  L_CS(GPIO);
  L_RS(GPIO);

  L_SCK(OUT);
  L_SDA(OUT);
  L_RST(OUT);
  L_CS(OUT);
  L_RS(OUT);

  SELA45(GPIO);
  SELA45(OUT);
  SELA45(SET);

  L_RST(SET);
  delay_ms(20); // Ждать стабилизации напряжений
  L_CS(RES);          // CS Выбор дисплея
  send_command(SLPOUT);      // Проснуться
  delay_ms(20); // Ждать стабилизации напряжений

  send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));

  send_command(MADCTL);
  send_byte(
  #ifdef EX_X_Y
    0x20 |
  #endif
  #ifdef FLIP_X
    0x40 |
  #endif
  #ifdef FLIP_Y
    0x80 |
  #endif
    0x00);

  send_command(COLMOD);
  send_byte(RGB_FORMAT);
  send_command(DISPON); // Display On

  L_CS(SET);
}

// Реализация виртуальных методов класса GFX

void ST7735::pixel(int32_t x, int32_t y)
{
  if (x > MAX_X || y > MAX_Y) return;
  L_CS(RES);

  set_addr(x, y, x, y);
  send_rgb(_color);
  send_rgb(_color);

  L_CS(SET);
}


// Реализация интерфейса PrintF

void ST7735::symbol(uint8_t *source, uint16_t x, uint16_t y, int32_t dx, int32_t dy)
{
  L_CS(RES);

  uint16_t x1 = x + dx - 1;
  uint16_t y1 = y + dy - 1;
  set_addr(x, y, x1, y1);

  for (int32_t j = 0; j < dy; j++) {
    uint32_t offset = (uint32_t)source + (j >> 3) * dx;
    int32_t bit = 1 << (j & 7);
    for (int32_t i = 0; i < dx; i++) {
      uint8_t data = *(uint8_t *)(offset + i);
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
    }
  }

  L_CS(SET);
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30

void ST7735::demo(int32_t d)
{
  L_CS(RES);
  set_addr(0, 0, MAX_X, MAX_Y);
  for (int32_t y = VIEWPORT_OFFSET; y < MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (int32_t x = VIEWPORT_OFFSET; x < MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      int32_t e = d << 2;
      uint16_t r = ((xx + yy) >> 6) + e;
      uint16_t g = ((yy - xx) >> 6) + e;
      uint16_t b = ((x * y) >> 6) - e;

      send_rgb(RGB(r, g, b));
    }
  }
  L_CS(SET);
}

void ST7735::test(int32_t k)
{
  L_CS(RES);
  set_addr(0, 0, MAX_X, MAX_Y);
  for (int32_t y = 0; y < MAX_Y + 1; y++)
    for (int32_t x = 0; x < MAX_X + 1; x++) {
      send_rgb(RGB(x << 1, y << 1, k));
    }
  L_CS(SET);
}
