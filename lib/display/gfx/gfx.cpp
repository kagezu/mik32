#include "gfx.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }
#define abs(x) ((x)>0?(x):-(x))

// template<typename T>
// void GFX::w_line(T x, T y, T x1)
void GFX::w_line(int16_t x, int16_t y, int16_t x1)
{
  // if (x > x1) swap(x, x1);

  if (x > (int16_t)max_x()) x = 0;
  if (x1 > (int16_t)max_x()) x1 = (int16_t)max_x();
  if (x == x1) pixel(x, y);
  else  rect(x, y, x1, y);
}

void GFX::h_line(int16_t x, int16_t y, int16_t y1)
{
  // if (y > y1) swap(y, y1);

  if (y > (int16_t)max_y()) y = 0;
  if (y1 > (int16_t)max_y()) y1 = (int16_t)max_y();
  if (y == y1) pixel(x, y);
  else  rect(x, y, x, y1);
}

void GFX::w_line_fat(int16_t x, int16_t y, int16_t x1)
{
  int16_t y1 = y + ((_fat + 1) >> 1) - 1;
  y -= _fat >> 1;
  if (y > (int16_t)max_y()) y = 0;
  if (y1 > (int16_t)max_y()) y1 = (int16_t)max_y();
  rect(x, y, x1, y1);
}

void GFX::h_line_fat(int16_t x, int16_t y, int16_t y1)
{
  int16_t x1 = x + ((_fat + 1) >> 1) - 1;
  x -= _fat >> 1;
  if (x > (int16_t)max_x()) x = 0;
  if (x1 > (int16_t)max_x()) x1 = (int16_t)max_x();
  rect(x, y, x1, y1);
}

void GFX::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx = x1 - x0;
  int16_t dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t yStep = 1;
  if (y0 > y1) yStep = -1;

  while (x0 <= x1) {
    if (steep)
      pixel(y0, x0++);
    else
      pixel(x0++, y0);

    err -= dy;
    if (err < 0) {
      y0 += yStep;
      err += dx;
    }
  }
}
void GFX::circle(int16_t x, int16_t y, int16_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x1 = 0;
  int16_t y1 = radius;

  pixel(x, y + radius);
  pixel(x, y - radius);
  pixel(x + radius, y);
  pixel(x - radius, y);

  while (x1 < y1) {
    if (f >= 0) {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;

    pixel(x + x1, y + y1);
    pixel(x - x1, y + y1);
    pixel(x + x1, y - y1);
    pixel(x - x1, y - y1);
    pixel(x + y1, y + x1);
    pixel(x - y1, y + x1);
    pixel(x + y1, y - x1);
    pixel(x - y1, y - x1);
  }
}

void GFX::circle_fat(int16_t x, int16_t y, int16_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  int16_t d0 = (_fat >> 1);
  int16_t d1 = ((_fat + 1) >> 1) - 1;
  int16_t x1 = x - d0;
  int16_t y1 = y - d0;
  int16_t x2 = x + d1;
  int16_t y2 = y + d1;

  circle(x, y, radius - d0);
  circle(x, y, radius + d1);

  h_line(x, y1 + radius, y2 + radius);
  h_line(x, y1 - radius, y2 - radius);
  w_line(x1 + radius, y, x2 + radius);
  w_line(x1 - radius, y, x2 - radius);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    h_line(x - dx, y1 - dy, y2 - dy);
    h_line(x + dx, y1 - dy, y2 - dy);
    h_line(x - dx, y1 + dy, y2 + dy);
    h_line(x + dx, y1 + dy, y2 + dy);

    w_line(x1 - dy, y - dx, x2 - dy);
    w_line(x1 - dy, y + dx, x2 - dy);
    w_line(x1 + dy, y - dx, x2 + dy);
    w_line(x1 + dy, y + dx, x2 + dy);
  }
}

void GFX::circle_fill(int16_t x, int16_t y, int16_t radius)
{
  radius++;
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  w_line(x - radius + 1, y, x + radius - 1);
  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
      w_line(x - dx + 1, y + dy, x + dx - 1);
      w_line(x - dx + 1, y - dy, x + dx - 1);
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    w_line(x - dy + 1, y + dx, x + dy - 1);
    w_line(x - dy + 1, y - dx, x + dy - 1);
  }
}

void GFX::round_rect_fill(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  int16_t x1 = x + radius;
  int16_t y1 = y + radius;
  int16_t x2 = x + width - radius - 1;
  int16_t y2 = y + height - radius - 1;

  rect(x, y1, x + width, y2);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;

      w_line(x1 - dx, y1 - dy, x2 + dx);
      w_line(x1 - dx, y2 + dy, x2 + dx);
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    w_line(x1 - dy, y1 - dx, x2 + dy);
    w_line(x1 - dy, y2 + dx, x2 + dy);
  }
}

void GFX::round_rect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  int16_t x1 = x + radius;
  int16_t y1 = y + radius;
  int16_t x2 = x + width - radius - 1;
  int16_t y2 = y + height - radius - 1;

  w_line(x1, y, x2);
  w_line(x1, y + height - 1, x2);
  h_line(x, y1, y2);
  h_line(x + width - 1, y1, y2);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    pixel(x1 - dx, y1 - dy);
    pixel(x1 - dy, y1 - dx);
    pixel(x2 + dx, y1 - dy);
    pixel(x2 + dy, y1 - dx);
    pixel(x1 - dx, y2 + dy);
    pixel(x1 - dy, y2 + dx);
    pixel(x2 + dx, y2 + dy);
    pixel(x2 + dy, y2 + dx);
  }
}

void GFX::line_fat(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx = x1 - x0;
  int16_t dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t yStep = 1;
  if (y0 > y1) yStep = -1;

  y1 = y0 + ((_fat + 1) >> 1) - 1;
  y0 -= _fat >> 1;

  while (x0 <= x1) {
    if (steep)
      w_line(y0, x0++, y1);
    else
      h_line(x0++, y0, y1);

    err -= dy;
    if (err < 0) {
      y0 += yStep;
      y1 += yStep;
      err += dx;
    }
  }
}

void GFX::rect_line(int16_t x, int16_t y, int16_t width, int16_t height)
{
  w_line(x, y, x + width - 1);
  w_line(x, y + height - 1, x + width - 1);
  h_line(x, y + 1, y + height - 2);
  h_line(x + width - 1, y + 1, y + height - 2);
}

void GFX::rect_fat(int16_t x, int16_t y, int16_t width, int16_t height)
{
  int16_t d0 = _fat >> 1;
  int16_t d1 = ((_fat + 1) >> 1);
  w_line_fat(x - d0, y, x + width - 2 + d1);
  w_line_fat(x - d0, y + height - 1, x + width - 2 + d1);
  h_line_fat(x, y + d0, y + height - d1);
  h_line_fat(x + width - 1, y + d0, y + height - d1);
}
