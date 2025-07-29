#pragma once
#include "printf.h"
#include "gfx/gfx.h"
#include "ST7735_SOFT/driver.h"  
#include "ILI9225_SOFT/driver.h"  
#include "ILI9225/driver.h"     
#include "ST7735/driver.h" 
#include "ST7789/driver.h"  
#include "ILI9486_8/driver.h" 
#include "SSD1306/driver.h" 
#ifdef MIK32V2
#include "ILI9486_16/driver.h" 
#include "NT35510/driver.h" 
#endif

#define FONT_TAB_FACTOR     2

template<typename Driver, const int R = R_0>
class Display : public Driver, public PrintF, public GFX {

#ifdef __AVR__
  typedef uint8_t reg;
#else
  typedef uint32_t reg;
#endif  

  // Driver =============================================================================

public:
  void init()
    // Display()
  {
    Driver::init(R);
    color(0xFFFFFF);
    background(0);
    viewport();
    clear();
  }

  void scroll(uint16_t sl)
  {
    select();
    send_command(VSCRSADD);
    send_word(sl);
    release();
  }

private:
  typename Driver::RGB _color;         // Цвет
  typename Driver::RGB _color2;        // Цвет 2
  typename Driver::RGB _background;    // Фон
  Rect *_viewport;  // Область вывода
  Rect _didplay = Rect(0, 0, max_x(), max_y());

  using Driver::set_addr;
  using Driver::send_rgb;
  using Driver::send_command;
  using Driver::send_word;
  using Driver::select;

  // gfx ================================================================================

public:
  using Driver::area;
  using Driver::pixel;
  using Driver::release;

  ATTR_INLINE constexpr int16_t max_x() { return R & EX_X_Y ? Driver::max_y() : Driver::max_x(); }
  ATTR_INLINE constexpr int16_t max_y() { return R & EX_X_Y ? Driver::max_x() : Driver::max_y(); }

  ATTR_INLINE  void color(typename Driver::RGB c) { _color = c; }
  ATTR_INLINE  void color2(typename Driver::RGB c) { _color2 = c; }
  ATTR_INLINE  void background(typename Driver::RGB b) { _background = b; }
  ATTR_INLINE  void clear() { area(0, 0, max_x(), max_y(), _background); }
  ATTR_INLINE  void fill(typename Driver::RGB color) { area(_viewport->min_x, _viewport->min_y, _viewport->max_x, _viewport->max_y, color); }
  ATTR_INLINE  void fill(Rect view) { area(view.min_x, view.min_y, view.max_x, view.max_y, _color); }
  ATTR_INLINE  void fill(Rect view, typename Driver::RGB color) { area(view.min_x, view.min_y, view.max_x, view.max_y, color); }
  ATTR_INLINE  void viewport(Rect *view) { _viewport = view; }
  ATTR_INLINE  void viewport() { _viewport = &_didplay; }

  void fill(int16_t x, int16_t y, int16_t x1, int16_t y1)
  {
    if (!_viewport->cut(x, y, x1, y1)) return;
    if (x == x1 && y == y1) pixel(x, y, _color);
    else area(x, y, x1, y1, _color);
  }

  void pixel(int16_t x, int16_t y)
  {
    if (!_viewport->is(x, y)) return;
    pixel(x, y, _color);
  }

  // print ==============================================================================

private:
  Font  _font;                            // Шрифт
  uint8_t _charSize = 0;                  // Размер символа в словах
  uint8_t _interline = 0;                 // Расстояние между строками
  uint8_t _interval = 0;                  // Расстояние между символами
  int16_t point_x = 0;
  int16_t point_y = 0;

public:
  void font(const Font &f, uint8_t w = 1, uint8_t h = 2)
  {
  #ifdef __AVR__
    memcpy_P(&_font, &f, sizeof(Font));
    _charSize = ((_font.weight * _font.height - 1) >> 3) + 1;
  #else
    *(int32_t *)&_font = *(int32_t *)&f;
    _font.offset = f.offset;
    _font.w = f.w;
    _font.data = f.data;

    _charSize = (((_font.weight * _font.height - 1) >> 5) + 1) << 2;
  #endif
    set_interline(h);
    set_interval(w);
  }

  ATTR_INLINE  void at(uint16_t x, uint16_t y) { point_x = _viewport->min_x + x; point_y = _viewport->min_y + y; }
  ATTR_INLINE  void set_interline(uint8_t interline) { _interline = _font.height + interline; }
  ATTR_INLINE  void set_interval(uint8_t interval) { _interval = interval; }
  ATTR_INLINE  uint8_t get_height() { return _interline; }
  ATTR_INLINE  uint8_t get_weight() { return _font.weight + _interval; }

  ATTR_NOINLINE void putc(uint8_t ch)
  {
    switch ((uint8_t)ch) {
      // Символ в русской кодировке, пропускаем префикс
      case 0xd0: break;
      case 0xd1: break;
        // Новая страница
      case '\f':
        point_x = _viewport->min_x;
        point_y = _viewport->min_y;
        break;
        // Перевод строки с возвратом
      case '\n':
        point_x = _viewport->min_x;
        point_y += _interline;
        if (point_y + _font.height > _viewport->max_y + 1) { point_y = _viewport->min_y; }
        break;
        // Возврат каретки
      case '\r':
        point_x = _viewport->min_x;
        break;
        // Шаг назад
      case '\b':
        point_x -= (_font.weight + _interval);
        if (point_x < _viewport->min_x) point_x = _viewport->min_x;
        break;
        // Табуляция
      case '\t':
        point_x = ((point_x / ((_font.weight + _interval) << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR;
        break;
        // Вертикальная табуляция / Перевод строки
      case '\v':
        point_y += _interline;
        if (point_y + _font.height > _viewport->max_y + 1) { point_y = _viewport->min_y; }
        break;

      case '\e':
        break;
      case '\1':
        { typename Driver::RGB tmp = _color; _color = _color2; _color2 = tmp; }
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

          if (point_x + dx > _viewport->max_x + 1) {
            point_y += _interline;
            point_x = _viewport->min_x;
          }
          if (point_y + _font.height > _viewport->max_y + 1) { point_x = _viewport->min_x; point_y = _viewport->min_y; }
          if (ch) symbol((reg *)source, point_x, point_y, dx, _font.height);
          else area(point_x, point_y, point_x + dx + _interval - 1, point_y + _font.height - 1, _background);
          point_x += dx + _interval;
        }
    }
  }

private:
  void symbol(reg *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
  {
    reg bit = 0;
    reg data = 0;

    select();
    set_addr(x, y, x + dx + _interval - 1, y + dy - 1);
    for (reg j = 0; j < dy; j++) {
      for (reg i = 0; i < dx; i++) {
      #ifdef __AVR__
        if (bit == 0) { bit = 1; data = pgm_read_byte(source++); }
      #else
        if (bit == 0) { bit = 1; data = *source++; }
      #endif
        if (data & bit)send_rgb(_color);//pixel(x + i, y + j, _color); 
        else send_rgb(_background);//pixel(x + i, y + j, _background);
        bit <<= 1;
      }
      // for (reg i = 0; i < _interval; i++) send_rgb(_background);
      if (_interval) send_rgb(_background, _interval);
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
    uint16_t yy = 0;
    for (int16_t y = 0; y < max_y() + 1; y++) {
      uint16_t xx = 0;
      uint16_t xy = 0;
      for (int16_t x = 0; x < max_x() + 1; x++) {

        int8_t e = d << 2;
        int8_t r = ((xx + yy) >> div) + e;
        int8_t g = ((yy - xx) >> div) + e;
        int8_t b = (xy >> div) - e;

        xy += y;  // Заменяем умножение сложением
        xx += x;

        send_rgb(typename Driver::RGB(r, g, b));
      }
      yy += y;
    }
    release();
  }

  void demo2(uint8_t d)
  {
    static const uint8_t div = 4 + ((max_x() + max_y()) >> 8);

    select();
    uint16_t yy = 0;
    for (int16_t y = 0; y < max_y() + 1; y++) {
      set_addr(0, y, max_x(), y);
      uint16_t xx = 0;
      uint16_t xy = 0;
      for (int16_t x = 0; x < max_x() + 1; x++) {

        int8_t e = d << 2;
        int8_t r = ((xx + yy) >> div) + e;
        int8_t g = ((yy - xx) >> div) + e;
        int8_t b = (xy >> div) - e;

        xy += y;  // Заменяем умножение сложением
        xx += x;

        send_rgb(typename Driver::RGB(r, g, b));
      }
      yy += y;
    }
    release();
  }
};
