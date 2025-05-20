#pragma once
#include "print/printf.h"
#include "gfx/gfx.h"

#define FONT_TAB_FACTOR     2

template<typename Driver, typename C>
class CDisplay : public Driver, public PrintF, public GFX {
private:
  C _color = 0x00ffffff;                    // Цвет
  C _background = 0;                        // Фон
  Font  _font = {};                         // Шрифт
  uint8_t  _charSize = 0;                   // Размер символа в байтах
  uint8_t  _line = 0;                       // Высота символа в байтах
  uint8_t  _interline = 0;                  // Расстояние между строками
  uint8_t  _interval = 0;                   // Расстояние между символами
  uint8_t  _tab_factor = FONT_TAB_FACTOR;
  uint16_t point_x = 0;
  uint16_t point_y = 0;

  using Driver::set_addr;
  using Driver::send_rgb;
  using Driver::send_command;
  using Driver::send_byte;
  using Driver::select;
  using Driver::release;

  void send_config(const uint8_t *config, uint8_t size)
  {
    while (size) {
      uint8_t count = pgm_read_byte(config++);
      uint8_t comand = pgm_read_byte(config++);
      size -= 2 + count;
      send_command(comand);
      while (count--) send_byte(pgm_read_byte(config++));
    }
  }

  // gfx ================================================================================

public:
  using Driver::area;
  using Driver::max_x;
  using Driver::max_y;

  GCC_INLINE inline void color(C c) { _color = c; }
  GCC_INLINE inline void background(C b) { _background = b; }
  GCC_INLINE inline void clear() { area(0, 0, max_x(), max_y(), _background); }
  GCC_INLINE inline void clear(C color) { area(0, 0, max_x(), max_y(), color); }

  void rect(int16_t x, int16_t y, int16_t x1, int16_t y1)
  {
    if (x > max_x()) x = max_x();
    if (x1 > max_x()) x1 = max_x();
    if (y > max_y()) y = max_y();
    if (y1 > max_y()) y1 = max_y();
    if (x == x1 && y == y1) pixel(x, y);
    else area(x, y, x1, y1, _color);
  }

  void pixel(int16_t x, int16_t y)
  {
    if (x > max_x() || y > max_y()) return;
    select();
    set_addr(x, y, x, y);
    send_rgb(_color);
    send_rgb(_color);
    release();
  }

  void pixel(int16_t x, int16_t y, C color)
  {
    if (x > max_x() || y > max_y()) return;
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }

  // print

  void font(const Font &font)
  {
  #ifdef MIK32V2
    _font = font;
  #else
    memcpy_P(&_font, &font, sizeof(Font));
  #endif
    _line = (1 + ((_font.height - 1) >> 3));
    _charSize = _font.weight * _line;
    set_interline(2);
    set_interval(1);
  }

  GCC_INLINE inline void at(uint16_t x, uint16_t y) { point_x = x; point_y = y; }
  GCC_INLINE inline void set_interline(uint8_t interline) { _interline = _font.height + interline; }
  GCC_INLINE inline void set_interval(uint8_t interval) { _interval = interval; }
  GCC_INLINE inline uint8_t get_height() { return _font.height; }
  GCC_INLINE inline uint8_t get_weight() { return _font.weight; }

  void putc(char ch)
  {
    switch ((uint8_t)ch) {
      // Символ в русской кодировке, пропускаем префикс
      case 0xd0: break;
      case 0xd1: break;
        // Новая страница
      case '\f':
        point_x = point_y = 0;
        break;
        // Перевод строки с возвратом
      case '\n':
        point_x = 0;
        point_y += _interline;
        if (point_y + _font.height > max_y()) { point_y = 0; }
        break;
        // Возврат каретки
      case '\r':
        point_x = 0;
        break;
        // Шаг назад
      case '\b':
        point_x -= (_font.weight + _interval);
        if (point_x > max_x()) point_x = 0;
        break;
        // Табуляция
      case '\t':
        point_x = ((point_x / ((_font.weight + _interval) << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR;
        break;
        // Вертикальная табуляция / Перевод строки
      case '\v':
        point_y += _interline;
        if (point_y + _font.height > max_y()) { point_y = 0; }
        break;

      case '\e':
        break;
      case '\0':
        point_x += _font.weight + _interval;
        break;

      default: output(ch);
    }
  }

private:
  void output(uint8_t ch)
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

  // Вывод символа (двух цветного изображения) на экран
  void symbol(uint8_t *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
  {
    uint16_t x1 = x + dx - 1;
    uint16_t y1 = y + dy - 1;

    select();
    set_addr(x, y, x1, y1);

    for (uint8_t j = 0; j < dy; j++) {
      uint8_t *offset = source + (j >> 3) * dx;
      uint8_t bit = 1 << (j & 7);
      for (uint8_t i = 0; i < dx; i++) {
        uint8_t data = pgm_read_byte(offset + i);
        if (data & bit) send_rgb(_color);
        else send_rgb(_background);
      }
    }
    release();
  }

  // тестирование дисплея ===============================================================

public:
  void demo(uint8_t d)
  {
    static const uint8_t div = 4 + ((max_x() + max_y()) >> 8);

    select();
    set_addr(0, 0, max_x(), max_y());
    uint16_t yy = 0;
    for (uint16_t y = 0; y < max_y() + 1; y++) {
      uint16_t xx = 0;
      uint16_t xy = 0;
      for (uint16_t x = 0; x < max_x() + 1; x++) {

        uint8_t e = d << 2;
        uint8_t r = ((xx + yy) >> div) + e;
        uint8_t g = ((yy - xx) >> div) + e;
        uint8_t b = (xy >> div) - e;

        xy += y;  // Заменяем умножение сложением
        xx += x;

        send_rgb(C(r, g, b));
      }
      yy += y;
    }
    release();
  }
};
