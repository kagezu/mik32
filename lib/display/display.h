#pragma once
#include "type/include.h"
#include "print/printf.h"
#include "gfx/gfx.h"

#define FONT_TAB_FACTOR     2

template<typename Driver, typename C = RGB16, const int R = ROT_0>
class Display : public Driver, public PrintF, public GFX {

  // Driver =============================================================================

public:
  void init()
  {
    Driver::init(R);
    color(0xffffff);
    background(0);
    viewport();
    clear();
  }

  void scroll(uint16_t sl)
  {
    select();
    send_command(VSCRSADD);
    send_byte(sl >> 8);
    send_byte(sl);
    release();
  }

private:
  C _color;       // Цвет
  C _background;  // Фон
  Rect _viewport; // Область вывода

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

  GCC_INLINE constexpr int16_t max_x() { return R & EX_X_Y ? Driver::max_y() : Driver::max_x(); }
  GCC_INLINE constexpr int16_t max_y() { return R & EX_X_Y ? Driver::max_x() : Driver::max_y(); }

  GCC_INLINE inline void color(C c) { _color = c; }
  GCC_INLINE inline void background(C b) { _background = b; }
  GCC_INLINE inline void clear() { area(0, 0, max_x(), max_y(), _background); }
  GCC_INLINE inline void fill(C color) { area(_viewport.min_x, _viewport.min_y, _viewport.max_x, _viewport.max_y, color); }
  GCC_INLINE inline void fill(Rect view) { area(view.min_x, view.min_y, view.max_x, view.max_y, _color); }
  GCC_INLINE inline void fill(Rect view, C color) { area(view.min_x, view.min_y, view.max_x, view.max_y, color); }
  GCC_INLINE inline void viewport(Rect &view) { _viewport = view; }
  GCC_INLINE inline void viewport() { _viewport = Rect(0, 0, max_x(), max_y()); }

  void fill(int16_t x, int16_t y, int16_t x1, int16_t y1)
  {
    if (!_viewport.cut(x, y, x1, y1)) return;
    if (x == x1 && y == y1) pixel(x, y);
    else area(x, y, x1, y1, _color);
  }

  void pixel(int16_t x, int16_t y)
  {
    if (!_viewport.is(x, y)) return;
    select();
    set_addr(x, y, x, y);
    send_rgb(_color);
    send_rgb(_color);
    release();
  }

  void pixel(int16_t x, int16_t y, C color)
  {
    if (!_viewport.is(x, y)) return;
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }

  // print ==============================================================================

private:
  Font  _font = {};                        // Шрифт
  uint8_t _charSize = 0;                   // Размер символа в словах
  uint8_t _interline = 0;                  // Расстояние между строками
  uint8_t _interval = 0;                   // Расстояние между символами
  uint8_t _tab_factor = FONT_TAB_FACTOR;
  int16_t point_x = 0;
  int16_t point_y = 0;

public:
  void font(const Font &f, uint8_t w = 1, uint8_t h = 2)
  {
  #ifdef __AVR__
    memcpy_P(&_font, &f, sizeof(Font));
    _charSize = ((_font.weight * _font.height - 1) >> 3) + 1;
  #else
    _font = f;
    _charSize = (((_font.weight * _font.height - 1) >> 5) + 1) << 2;
  #endif
    set_interline(h);
    set_interval(w);
  }

  GCC_INLINE inline void at(uint16_t x, uint16_t y) { point_x = _viewport.min_x + x; point_y = _viewport.min_y + y; }
  GCC_INLINE inline void set_interline(uint8_t interline) { _interline = _font.height + interline; }
  GCC_INLINE inline void set_interval(uint8_t interval) { _interval = interval; }
  GCC_INLINE inline uint8_t get_height() { return _interline; }
  GCC_INLINE inline uint8_t get_weight() { return _font.weight + _interval; }

  GCC_NO_INLINE void putc(uint8_t ch)
  {
    switch ((uint8_t)ch) {
      // Символ в русской кодировке, пропускаем префикс
      case 0xd0: break;
      case 0xd1: break;
        // Новая страница
      case '\f':
        point_x = _viewport.min_x;
        point_y = _viewport.min_y;
        break;
        // Перевод строки с возвратом
      case '\n':
        point_x = _viewport.min_x;
        point_y += _interline;
        if (point_y + _font.height > _viewport.max_y + 1) { point_y = _viewport.min_y; }
        break;
        // Возврат каретки
      case '\r':
        point_x = _viewport.min_x;
        break;
        // Шаг назад
      case '\b':
        point_x -= (_font.weight + _interval);
        if (point_x < _viewport.min_x) point_x = _viewport.min_x;
        break;
        // Табуляция
      case '\t':
        point_x = ((point_x / ((_font.weight + _interval) << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR;
        break;
        // Вертикальная табуляция / Перевод строки
      case '\v':
        point_y += _interline;
        if (point_y + _font.height > _viewport.max_y + 1) { point_y = _viewport.min_y; }
        break;

      case '\e':
        break;
      case '\0':
        point_x += _font.weight + _interval;
        break;

      default:
        {
          ch -= _font.first_char;
          if (_font.count_char <= ch) ch = 0;

          uint8_t dx;
          uint8_t *source;

          if (_font.w) dx = pgm_read_byte(&_font.w[ch]);
          else dx = _font.weight;

          if (_font.offset) { source = (uint8_t *)_font.data + pgm_read_word(&_font.offset[ch]); }
          else { source = (uint8_t *)_font.data + ch * _charSize; }

          if (point_x + dx > _viewport.max_x + 1) {
            point_y += _interline;
            point_x = _viewport.min_x;
          }
          if (point_y + _font.height > _viewport.max_y + 1) { point_x = _viewport.min_x; point_y = _viewport.min_y; }
          symbol((reg *)source, point_x, point_y, dx, _font.height);
          point_x += dx + _interval;
        }
    }
  }

private:
  void symbol(reg *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
  {
    uint16_t count = dx * dy;
    reg bit = 0;
    reg data = 0;

    select();
    set_addr(x, y, x + dx - 1, y + dy - 1);
    while (count--) {
    #ifdef __AVR__
      if (bit == 0) { bit = 1; data = pgm_read_byte(source++); }
    #else
      if (bit == 0) { bit = 1; data = *source++; }
    #endif
      if (data & bit) send_rgb(_color); else send_rgb(_background);
      bit <<= 1;
    }
    release();
  }

  // тестирование =======================================================================

public:
  void demo(uint8_t d)
  {
    static const uint8_t div = 4 + ((max_x() + max_y()) >> 8);

    select();
    set_addr(0, 0, max_x(), max_y());
    int16_t yy = 0;
    for (int16_t y = 0; y < max_y() + 1; y++) {
      int16_t xx = 0;
      int16_t xy = 0;
      for (int16_t x = 0; x < max_x() + 1; x++) {

        int8_t e = d << 2;
        int8_t r = ((xx + yy) >> div) + e;
        int8_t g = ((yy - xx) >> div) + e;
        int8_t b = (xy >> div) - e;

        xy += y;  // Заменяем умножение сложением
        xx += x;

        send_rgb(C(r, g, b));
      }
      yy += y;
    }
    release();
  }
};
