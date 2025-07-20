#include "printf.h"
#include "macros/pgmspace.h"
// #include "string.h"

#define KEY_u
// #define KEY_d
// #define KEY_x
#define KEY_q
#define KEY_S
// #define KEY_p

#define NUM_16
// #define NUM_32
// #define NUM_64

#define LEFT_RIGHT   0x00
#define LEFT_ALGIN   0x80

// для mik32 должно быть чётным + 1 байт
#define PRINT_BUFFER_SIZE   33

char PrintF::buffer[PRINT_BUFFER_SIZE];

void PrintF::printf(const char *string, ...)
{
  char ch;
  __builtin_va_list args;
  __builtin_va_start(args, string);

  while ((ch = pgm_read_byte(string++))) {
    if (ch != '%') {
      putc(ch);
      continue;
    }

    char algin = LEFT_RIGHT, // выравнивание справа
      digit = 0,            // число цифр
      lng = 2,              // число байт
      fix = 0,              // цифр после запятой
      fix2 = 0;             // формат Q (lng * 8).fix

    char *ptr = &buffer[PRINT_BUFFER_SIZE - 1];
    ch = pgm_read_byte(string++);

    if (ch == '-') {
      algin = LEFT_ALGIN;   // выравнивание слева
      ch = pgm_read_byte(string++);
    }

    while (ch > '/' && ch < ':') {
      digit = digit * 10 + ch - '0';
      ch = pgm_read_byte(string++);
    }

    if (ch == '.') {
      ch = pgm_read_byte(string++);
      while (ch > '/' && ch < ':') {
        fix = fix * 10 + ch - '0';
        ch = pgm_read_byte(string++);
      }

      if (ch == '.') {
        ch = pgm_read_byte(string++);
        while (ch > '/' && ch < ':') {
          fix2 = fix2 * 10 + ch - '0';
          ch = pgm_read_byte(string++);
        }
      }
    }

    while (ch == 'l') {     // увеличить разрядность
      ch = pgm_read_byte(string++);
      lng <<= 1;
    }
    digit |= algin;

    switch (ch) {
    #ifdef KEY_u
      case 'u':
        switch (lng) {
        #ifdef NUM_16
          case 2: ptr = sprint(ptr, (uint16_t)(uint32_t)__builtin_va_arg(args, void *)); break;
          #endif
          #ifdef NUM_32
          case 4: ptr = sprint(ptr, (uint32_t)__builtin_va_arg(args, uint32_t)); break;
          #endif
          #ifdef NUM_64
          case 8: ptr = sprint(ptr, (uint64_t)__builtin_va_arg(args, uint64_t)); break;
          #endif
        } break;
      #endif

      #ifdef KEY_d
      case 'd':
        switch (lng) {
        #ifdef NUM_16
          case 2: ptr = sprint(ptr, (int16_t)(uint32_t)__builtin_va_arg(args, void *)); break;
          #endif
          #ifdef NUM_32
          case 4: ptr = sprint(ptr, (int32_t)__builtin_va_arg(args, int32_t)); break;
          #endif
          #ifdef NUM_64
          case 8: ptr = sprint(ptr, (int64_t)__builtin_va_arg(args, int64_t)); break;
          #endif
        } break;
      #endif

      #ifdef KEY_q
      case 'q':
        switch (lng) {
        #ifdef NUM_16
          case 2: ptr = sprint(ptr, (int16_t)(uint32_t)__builtin_va_arg(args, void *), fix, fix2); break;
          #endif
          #ifdef NUM_32
          case 4: ptr = sprint(ptr, (int32_t)__builtin_va_arg(args, int32_t), fix, fix2); break;
          #endif
          #ifdef NUM_64
          case 8: ptr = sprint(ptr, (int64_t)__builtin_va_arg(args, int64_t), fix, fix2); break;
          #endif
        } break;
      #endif

      #ifdef KEY_x
      case 'x':
        if (digit && (digit < 3)) lng = 1; // явное указание, что число короткое
        switch (lng) {
          case 1:  ptr -= 2; *(uint16_t *)ptr = h_print((uint8_t)(uint32_t)__builtin_va_arg(args, void *)); break;
          #ifdef NUM_16
          case 2: ptr = sprint(ptr, (uint16_t)(uint32_t)__builtin_va_arg(args, void *, lng); break;
          #endif
          #ifdef NUM_32
          case 4:  ptr = sprint(ptr, (uint32_t)__builtin_va_arg(args, uint32_t), lng); break;
          #endif
          #ifdef NUM_64
          case 8: ptr = sprint(ptr, (uint64_t)__builtin_va_arg(args, uint64_t), lng); break;
          #endif
        } break;
      #endif

      #ifdef KEY_S
      case 'S': prints((const char *)__builtin_va_arg(args, char *)); continue;
      #endif

      #ifdef KEY_p
      case 'p': ptr = sprint_h(ptr, (void *)__builtin_va_arg(args, void *); break;
      #endif

      case 'c': putc((char)(uint32_t)__builtin_va_arg(args, void *)); continue;
      case 's': prints((char *)__builtin_va_arg(args, char *), digit); continue;
      case '%': putc('%'); continue;
      default: putc('?'); continue;
    }
    prints(ptr, digit);
  }
  __builtin_va_end(args);
}

///////////////////////////////////////////////////////////////////////////////

void PrintF::prints(char *string, char algin)
{
  char flag = algin & LEFT_ALGIN;
  char count = algin - flag - strlen(string);
  count = count < 0 ? 0 : count;
  if (!flag) while (count--)putc(' ');
  while (char ch = *string++) putc(ch);
  if (flag) while (count--)putc(' ');
}

void PrintF::prints(const char *string)
{
  while (char ch = pgm_read_byte(string++)) putc(ch);
}

uint16_t PrintF::h_print(uint8_t number)
{
  uint8_t low = number & 0xf;
  uint8_t high = number >> 4;
  low = low > 9 ? low + '7' : low + '0';
  high = high > 9 ? high + '7' : high + '0';
  return high + (low << 8);
}

char PrintF::strlen(char *string)
{
  char count = 0;
  while (*string++) count++;
  return count;
}
