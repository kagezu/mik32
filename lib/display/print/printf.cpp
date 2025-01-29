// #include "printf.h"

// void PrintF::font(const Font *font)
// {
//   memcpy_P(&_font, font, sizeof(Font));
//   _line = (1 + ((_font.height - 1) >> 3));
//   _charSize = _font.weight * _line;
//   set_interline(2);
//   set_interval(1);
// }

// void PrintF::print(char ch)
// {
//   switch (ch) {
//     case '\f': point_x = point_y = 0; break;  // Новая страница
//     case '\n': LF(); CR(); break;             // Перевод строки с возвратом
//     case '\r': CR(); break;                   // Возврат каретки
//     case '\b': BS(); break;                   // Шаг назад
//     case '\t': TAB(); break;                  // Табуляция
//     case '\v': LF(); break;                   // Вертикальная табуляция / Перевод строки
//     case '\e': escape(); break;
//     case '\0': point_x += _font.weight + _interval; break;
//     default: if ((uint8_t)ch < 0xd0) letter(ch);
//   }
// }

// void PrintF::printf(const char *string, ...)
// {
//   char ch;
//   __builtin_va_list args;
//   __builtin_va_start(args, string);

//   while ((ch = pgm_read_byte(string++))) {
//     switch (ch) {
//       case '%': {
//           char arg = '0';
//           ch = pgm_read_byte(string++);
//           if (ch > '/' && ch < ':') {
//             arg = ch;
//             ch = pgm_read_byte(string++);
//           }
//           switch (ch) {
//             case 'c': print((char)__builtin_va_arg(args, int16_t)); break;
//             case 'd':
//             case 'i':
//               switch (arg) {
//                 case '0':
//                 case '2': print((int16_t)__builtin_va_arg(args, int16_t)); break;
//                 case '4': print((int32_t)__builtin_va_arg(args, int32_t)); break;
//               } break;
//             case 's': print((char *)__builtin_va_arg(args, char *)); break;
//             case 'S': print((const char *)__builtin_va_arg(args, char *)); break;
//             case 'u':
//               switch (arg) {
//                 case '0':
//                 case '2': print((uint16_t)__builtin_va_arg(args, uint16_t)); break;
//                 case '4': print((uint32_t)__builtin_va_arg(args, uint32_t)); break;
//               } break;
//             case 'x':
//               switch (arg) {
//                 case '0':
//                 case '1':  print_h((uint8_t)__builtin_va_arg(args, uint16_t)); break;
//                 case '2':  print_h((uint16_t)__builtin_va_arg(args, uint16_t)); break;
//                 case '4':  print_h((uint32_t)__builtin_va_arg(args, uint32_t)); break;
//                 case '8':  print_h((uint64_t)__builtin_va_arg(args, uint64_t)); break;
//               } break;
//             case 'p': print_h((uint16_t)__builtin_va_arg(args, uint16_t)); break;
//             case '%': print('%'); break;
//           } break;
//         }
//       default: if ((uint8_t)ch < 0xd0) print(ch);
//     }
//   }
//   __builtin_va_end(args);
// }

// void PrintF::print(char *string)
// {
//   while (char ch = *string++) if ((uint8_t)ch < 0xd0) print(ch);
// }

// void PrintF::print(const char *string)
// {
//   while (char ch = pgm_read_byte(string++)) if ((uint8_t)ch < 0xd0) print(ch);
// }

// void PrintF::print(int32_t number)
// {
//   if (number < 0) { print('-'); number = -number; }
//   print((uint32_t)number);
// }

// void PrintF::print(int16_t number)
// {
//   if (number < 0) { print('-'); number = -number; }
//   print((uint16_t)number);
// }

// void PrintF::print(uint32_t number)
// {
//   char str[11];
//   char *ptr = &str[10];
//   *ptr = 0;

//   while (number > 9) {
//     uint8_t mod;

//   #ifdef ACCEL
//     uint8_t tmp;
//     div10_32bit(number, mod, tmp);
//   #else
//     mod = number % 10;
//     number /= 10;
//   #endif

//     *--ptr = mod + '0';
//   }
//   *--ptr = number + '0';
//   print(ptr);

// }

// void PrintF::print(uint16_t number)
// {
//   char str[7];
//   char *ptr = &str[6];
//   *ptr = 0;

//   while (number > 9) {
//     uint8_t mod;

//   #ifdef ACCEL
//     uint8_t tmp;
//     div10_16bit(number, mod, tmp);
//   #else
//     mod = number % 10;
//     number /= 10;
//   #endif

//     *--ptr = mod + '0';
//   }
//   *--ptr = number + '0';
//   print(ptr);
// }

// void PrintF::print_h(uint64_t number)
// {
//   union { uint64_t val; struct { uint8_t a; uint8_t b; uint8_t c; uint8_t d; uint8_t e; uint8_t f; uint8_t g; uint8_t h; }; } out;
//   out.val = number;
//   print('#');
//   print_h(out.h);
//   print_h(out.g);
//   print_h(out.f);
//   print_h(out.e);
//   print_h(out.d);
//   print_h(out.c);
//   print_h(out.b);
//   print_h(out.a);
// }

// void PrintF::print_h(uint32_t number)
// {
//   print('#');
//   print_h(to_byte(number, 3));
//   print_h(to_byte(number, 2));
//   print_h(to_byte(number, 1));
//   print_h(to_byte(number, 0));
// }

// void PrintF::print_h(uint16_t number)
// {
//   print('#');
//   print_h(to_byte(number, 1));
//   print_h(to_byte(number, 0));
// }

// void PrintF::print_h(uint8_t number)
// {
//   uint8_t low = number & 0xf;
//   uint8_t high = number >> 4;
//   letter(high > 9 ? high + '7' : high + '0');
//   letter(low > 9 ? low + '7' : low + '0');
// }
