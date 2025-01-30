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

  void font(const Font &);
  void at(uint16_t x, uint16_t y) { point_x = x; point_y = y; }
  void set_interline(uint8_t interline) { _interline = _font.height + interline; }
  void set_interval(uint8_t interval) { _interval = interval; }
  uint8_t get_height() { return _font.height; }
  uint8_t get_weight() { return _font.weight; }
  uint8_t get_row() { return (max_y() + 1) / _interline; }
  uint8_t get_col() { return (max_x() + 1) / (_interval + _font.weight); }

  // virtual void symbol(uint8_t *, uint16_t, uint16_t, uint8_t, uint8_t);
  // virtual const uint16_t max_x() = 0;
  // virtual const uint16_t max_y() = 0;
  // using IDisplay::max_x;
  // using IDisplay::max_y;
  // using IDisplay::symbol;

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

public:
  void letter(uint8_t ch)
  {
    ch -= _font.first_char;
    if (_font.count_char <= ch) ch = 0;

    uint8_t dx = _font.weight;
    uint8_t *source = (uint8_t *)_font.data;

    if (_font.offset) {
      uint16_t *index = (uint16_t *)_font.offset + ch;
      uint16_t offset = pgm_read_word(index);
      dx = (pgm_read_word(index + 1) - offset) / _line;
      source += offset;
    }
    else
      source = (uint8_t *)_font.data + ch * _charSize;


    if (point_x + dx > max_x()) {
      point_y += _interline;
      point_x = 0;
    }
    if (point_y + _font.height > max_y()) { point_x = point_y = 0; }
    symbol((uint8_t *)source, point_x, point_y, dx, _font.height);
    point_x += dx + _interval;
  }
};
