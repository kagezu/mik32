#pragma once
#include "inttypes.h"

class Rect {
public:

  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  int16_t width;
  int16_t height;

public:
  Rect() :min_x(0), min_y(0), max_x(0), max_y(0), width(1), height(1) {}
  Rect(int16_t x, int16_t y) : min_x(0), min_y(0), max_x(x), max_y(y), width(x + 1), height(y + 1) {}
  Rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) : min_x(x0), min_y(y0), max_x(x1), max_y(y1), width(x1 - x0 + 1), height(y1 - y0 + 1)
  {}

  bool cut(Rect &rect)
  {
    if (rect.min_x < min_x) rect.min_x = min_x;
    if (rect.max_x > max_x) rect.max_x = max_x;
    if (rect.min_y < min_y) rect.min_y = min_y;
    if (rect.max_y > max_y) rect.max_y = max_y;
    rect.width = max_x - min_x + 1;
    rect.height = max_y - min_y + 1;
    return rect.width < 0 || rect.height < 0;
  }

  bool cut(int16_t &x0, int16_t &y0, int16_t &x1, int16_t &y1)
  {
    if (x0 < min_x) x0 = min_x;
    if (x1 > max_x) x1 = max_x;
    if (y0 < min_y) y0 = min_y;
    if (y1 > max_y) y1 = max_y;
    return x0 <= x1 && y0 <= y1;
  }

  bool is(int16_t x, int16_t y)
  {
    return x <= max_x && x >= min_x && y <= max_y && y >= min_y;
  }
};
