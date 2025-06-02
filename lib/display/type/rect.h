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

  union {
    uint32_t Z;
    struct {
      int16_t width;
      int16_t height;
    };
  };


public:
  Rect() : X(0), Y(0), width(0), height(0) {}
  Rect(int16_t x, int16_t y) : X(x), Y(y), width(x + 1), height(y + 1) {}
  Rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1) : X((x0 << 16) | x1), Y((y0 << 16) | y1), width(x1 - x0 + 1), height(y1 - y0 + 1)
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
