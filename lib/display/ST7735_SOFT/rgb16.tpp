template<>
void ST7735_SOFT<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
}
template<>
ATTR_INLINE void ST7735_SOFT<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
void ST7735_SOFT<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  uint16_t rgb = color.rgb;

  ST_SOFT_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

#ifdef __AVR__
  reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
  reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
  reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);

  // Дублирование кода намеренно, так как оптимизация ускоряет тут выполнение в 2 раза
  while (len--) {
    ST_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
  }
#else

  while (len--) {
    int mask = 0x8000;
    while (mask) {
      if (rgb & mask)  ST_SOFT_SDA(SET);
      else  ST_SOFT_SDA(CLR);
      ST_SOFT_SCK(SET);
      ST_SOFT_SCK(CLR);
      mask >>= 1;
    }
  }
#endif
  ST_SOFT_SCK(CLR);
  ST_SOFT_CS(SET);
}
