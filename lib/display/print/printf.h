// #pragma once
// #include "type/include.h"

// #define FONT_TAB_FACTOR     1

// class PrintF {
// public:
//   void printf(const char *, ...);
//   void print(const char *);
//   void print(char *);
//   void print(uint16_t);
//   void print(uint32_t);
//   void print(int16_t);
//   void print(int32_t);
//   void print_h(uint64_t);
//   void print_h(uint32_t);
//   void print_h(uint16_t);
//   void print_h(uint8_t);
//   void print(char);
//   void letter(uint8_t);

//   void font(const Font *);
//   void at(uint16_t x, uint16_t y) { point_x = x; point_y = y; }
//   void set_interline(uint8_t interline) { _interline = _font.height + interline; }
//   void set_interval(uint8_t interval) { _interval = interval; }
//   uint8_t get_height() { return _font.height; }
//   uint8_t get_weight() { return _font.weight; }
//   uint8_t get_row() { return ((uint16_t)MAX_Y + 1) / _interline; }
//   uint8_t get_col() { return ((uint16_t)MAX_X + 1) / (_interval + _font.weight); }

//   virtual void symbol(uint8_t *, uint16_t, uint16_t, uint8_t, uint8_t);
//   // inline  virtual uint16_t min_x() = 0;
//   // inline  virtual uint16_t min_y() = 0;
//   // inline  virtual uint16_t max_x() = 0;
//   // inline  virtual uint16_t max_y() = 0;


// private:
//   Font  _font = {};     // Шрифт
//   uint8_t  _charSize = 0;  // Размер символа в байтах
//   uint8_t  _line = 0;      // Высота символа в байтах
//   uint8_t  _interline = 0; // Расстояние между строками
//   uint8_t  _interval = 0;  // Расстояние между символами
//   uint8_t  _tab_factor = FONT_TAB_FACTOR;
//   uint16_t point_x = 0;
//   uint16_t point_y = 0;

// private:
//   void LF() { point_y += _interline; }
//   void CR() { point_x = 0; }
//   void TAB() { point_x = ((point_x / ((_font.weight + _interval) << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR; }
//   void BS() { point_x -= (_font.weight + _interval); if (point_x > MAX_X) point_x = 0; }
//   void escape() {}
// };

// void PrintF::letter(uint8_t ch)
// {
//   ch -= _font.first_char;
//   if (_font.count_char <= ch) ch = 0;

//   uint8_t dx = _font.weight;
//   uint16_t source;

//   if (_font.offset) {
//     uint16_t index = _font.offset + ch * 2;
//     source = pgm_read_word(index);
//     dx = (pgm_read_word(index + 2) - source) / _line;
//     source += _font.data;
//   }
//   else
//     source = _font.data + ch * _charSize;


//   if (point_x + dx > MAX_X) {
//     point_y += _interline;
//     point_x = 0;
//   }
//   if (point_y + _font.height > MAX_Y) { point_x = point_y = 0; }
//   symbol((uint8_t *)source, point_x, point_y, dx, _font.height);
//   point_x += dx + _interval;
// }
