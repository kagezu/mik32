#include "printf.h"
#include "accel.h"

#define LEFT_ALGIN   0x80

void PrintF::font(const Font &font)
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

void PrintF::print_c(uint8_t ch)
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

void PrintF::print(char ch)
{
  switch ((uint8_t)ch) {
    case 0xd0: break; // Символ в русской кодировке, пропускаем префикс
    case 0xd1: break;
    case '\f': point_x = point_y = 0; break;  // Новая страница
    case '\n': LF(); CR(); break;             // Перевод строки с возвратом
    case '\r': CR(); break;                   // Возврат каретки
    case '\b': BS(); break;                   // Шаг назад
    case '\t': TAB(); break;                  // Табуляция
    case '\v': LF(); break;                   // Вертикальная табуляция / Перевод строки
    case '\e': escape(); break;
    case '\0': point_x += _font.weight + _interval; break;
    default: print_c(ch);
  }
}

void PrintF::printf(const char *string, ...)
{
  char ch;
  __builtin_va_list args;
  __builtin_va_start(args, string);

  while ((ch = pgm_read_byte(string++))) {
    if (ch != '%') { print(ch); continue; }

    reg algin = 0, digit = 0, lng = 1;
    char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
    ch = pgm_read_byte(string++);
    if (ch == '-') {
      algin = LEFT_ALGIN;
      ch = pgm_read_byte(string++);
    }
    while (ch > '/' && ch < ':') {
      digit = digit * 10 + ch - '0';
      ch = pgm_read_byte(string++);
    }
    while (ch == 'l') { // увеличить разрядность
      ch = pgm_read_byte(string++);
      lng++;
    }
    digit |= algin;
    switch (ch) {
      case 'c': print((char)__builtin_va_arg(args, adr)); break;
      case 's': print((char *)__builtin_va_arg(args, char *), digit); break;
      case 'S': print((const char *)__builtin_va_arg(args, char *)); break;
      case 'd':
        switch (lng) {
          case 1: ptr = print((int16_t)__builtin_va_arg(args, adr)); break;
          case 2: ptr = print((int32_t)__builtin_va_arg(args, int32_t)); break;
        } print(ptr, digit); break;
      case 'u':
        switch (lng) {
          case 1: ptr = print((uint16_t)__builtin_va_arg(args, adr)); break;
          case 2: ptr = print((uint32_t)__builtin_va_arg(args, uint32_t)); break;
        } print(ptr, digit); break;
      case 'x':
        if (digit && (digit < 3)) lng = 0; // явное указание, что число короткое
        switch (lng) {
          case 0:  ptr--; *(uint16_t *)--ptr = print_h((uint8_t)__builtin_va_arg(args, adr)); break;
          case 1:  ptr = print_h((uint16_t)__builtin_va_arg(args, adr)); break;
          case 2:  ptr = print_h((uint32_t)__builtin_va_arg(args, uint32_t)); break;
          case 3:  ptr = print_h((uint64_t)__builtin_va_arg(args, uint64_t)); break;
        } print(ptr, digit); break;

      case 'p': ptr = print_h((adr)__builtin_va_arg(args, adr)); print(ptr, digit); break;
      case '%': print_c('%'); break;
    }
  }
  __builtin_va_end(args);
}

void PrintF::print(char *string, reg algin)
{
  reg flag = algin & LEFT_ALGIN;
  reg count = algin - flag - get_length(string);
  count = count > LEFT_ALGIN ? 0 : count;
  if (!flag) while (count--)print_c(' ');
  while (char ch = *string++) print(ch);
  if (flag) while (count--)print_c(' ');
}

void PrintF::print(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((uint8_t)ch < 0xd0) print(ch);
}

// char *PrintF::print(int64_t number)
// {
//   reg neg = number < 0;
//   number = neg ? -number : number;
//   char *ptr = print(number);
//   if (neg) *--ptr = '-';
//   return ptr;
// }

char *PrintF::print(int32_t number)
{
  reg neg = number < 0;
  number = neg ? -number : number;
  char *ptr = print((uint32_t)number);
  if (neg) *--ptr = '-';
  return ptr;
}

char *PrintF::print(int16_t number)
{
  reg neg = number < 0;
  number = neg ? -number : number;
  char *ptr = print((uint16_t)number);
  if (neg) *--ptr = '-';
  return ptr;
}

char *PrintF::print(uint32_t number)
{
  char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
  while (number > 9) {
    uint8_t mod;

  #ifdef __AVR__
    uint8_t tmp;
    div10_32bit(number, mod, tmp);
  #else
    mod = number % 10;
    number /= 10;
  #endif

    *--ptr = mod + '0';
  }
  *--ptr = number + '0';
  return ptr;
}

char *PrintF::print(uint16_t number)
{
  char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
  while (number > 9) {
    uint8_t mod;

  #ifdef __AVR__
    uint8_t tmp;
    div10_16bit(number, mod, tmp);
  #else
    mod = number % 10;
    number /= 10;
  #endif
    *--ptr = mod + '0';
  }
  *--ptr = number + '0';
  return ptr;
}

char *PrintF::print_h(uint64_t number)
{
  char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 0));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 1));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 2));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 3));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 4));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 5));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 6));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 7));
  return ptr;
}

char *PrintF::print_h(uint32_t number)
{
  char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 0));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 1));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 2));
  ptr--; *(uint16_t *)--ptr = print_h(to_byte(number, 3));
  return ptr;
}

char *PrintF::print_h(uint16_t number)
{
  char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
  ptr--; *(uint16_t *)(--ptr) = print_h(to_byte(number, 0));
  ptr--; *(uint16_t *)(--ptr) = print_h(to_byte(number, 1));
  return ptr;
}

uint16_t PrintF::print_h(uint8_t number)
{
  uint8_t low = number & 0xf;
  uint8_t high = number >> 4;
  low = low > 9 ? low + '7' : low + '0';
  high = high > 9 ? high + '7' : high + '0';
  return high + (low << 8);
}

reg PrintF::get_length(char *string)
{
  reg count = 0;
  while (*string++) count++;
  return count;
}
