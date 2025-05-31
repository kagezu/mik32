#pragma once
#include <inttypes.h>

class Rect {
public:
  union {
    uint32_t X;
    struct {
      int16_t max_x;
      int16_t min_x;
    };
  };

  union {
    uint32_t Y;
    struct {
      int16_t max_y;
      int16_t min_y;
    };
  };

public:
  Rect() : X(0), Y(0) {}
  Rect(int16_t x, int16_t y) : X(x), Y(y) {}
  Rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) : X((x0 << 16) | x1), Y((y0 << 16) | y1) {}

  bool cut(Rect &rect)
  {
    if (rect.min_x < min_x) rect.min_x = min_x;
    if (rect.max_x > max_x) rect.max_x = max_x;
    if (rect.min_y < min_y) rect.min_y = min_y;
    if (rect.max_y > max_y) rect.max_y = max_y;
    return rect.min_x <= rect.max_x || rect.min_y <= rect.max_y;
  }

  bool cut(int16_t &x0, int16_t &y0, int16_t &x1, int16_t &y1)
  {
    if (x0 < min_x) x0 = min_x;
    if (x1 > max_x) x1 = max_x;
    if (y0 < min_y) y0 = min_y;
    if (y1 > max_y) y1 = max_y;
    return x0 <= x1 || y0 <= y1;
  }

  bool is(int16_t x, int16_t y)
  {
    return x <= max_x && x >= min_x && y <= max_y && y >= min_y;
  }
};
