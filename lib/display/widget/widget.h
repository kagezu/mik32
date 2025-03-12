#pragma once
#include "core.h"
#include "type/buffer.h"
// #include "display.h"


enum GFX : uint8_t {

  GFX_w_line = 0,
  GFX_h_line = 1,
  GFX_w_line_fat = 2,
  GFX_h_line_fat = 3,
  GFX_line = 4,
  GFX_round_rect = 5,
  GFX_rect_line = 6,
  GFX_circle = 7,
  GFX_line_fat = 8,
  GFX_rect_fat = 9,
  GFX_circle_fat = 10,
  GFX_circle_fill = 11,
  GFX_round_rect_fill = 12
};


class Widget {
public:
  Widget() {}

  void set(uint16_t x, uint16_t y) { X = x; Y = y; }
  // void update();
  // void draw();

protected:
  uint16_t X;
  uint16_t Y;
  uint16_t *graph;

  void send(uint16_t);
  uint16_t read();
};
