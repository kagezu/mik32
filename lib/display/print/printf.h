#pragma once
#include "type/include.h"

#define FONT_TAB_FACTOR     1

class PrintF :private IDisplay {
public:
  void printf(const char *, ...);
  void print(const char *);
  void print(char *);
  void print(uint16_t);
  void print(uint32_t);
  void print(int16_t);
  void print(int32_t);
  void print_h(uint64_t);
  void print_h(uint32_t);
  void print_h(uint16_t);
  void print_h(uint8_t);
  void print(char);
  void letter(uint8_t);

  void font(const Font &);
  void at(uint16_t x, uint16_t y) { point_x = x; point_y = y; }
  void set_interline(uint8_t interline) { _interline = _font.height + interline; }
  void set_interval(uint8_t interval) { _interval = interval; }
  uint8_t get_height() { return _font.height; }
  uint8_t get_weight() { return _font.weight; }
  uint8_t get_row() { return (max_y() + 1) / _interline; }
  uint8_t get_col() { return (max_x() + 1) / (_interval + _font.weight); }

private:
  Font  _font = {};     // Шрифт
  uint8_t  _charSize = 0;  // Размер символа в байтах
  uint8_t  _line = 0;      // Высота символа в байтах
  uint8_t  _interline = 0; // Расстояние между строками
  uint8_t  _interval = 0;  // Расстояние между символами
  uint8_t  _tab_factor = FONT_TAB_FACTOR;
  uint16_t point_x = 0;
  uint16_t point_y = 0;

private:
  void LF() { point_y += _interline; }
  void CR() { point_x = 0; }
  void TAB() { point_x = ((point_x / ((_font.weight + _interval) << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR; }
  void BS() { point_x -= (_font.weight + _interval); if (point_x > max_x()) point_x = 0; }
  void escape() {}
};
