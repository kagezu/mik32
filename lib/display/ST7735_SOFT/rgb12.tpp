template<>
void ST7735_SOFT<RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
}

template<>
void ST7735_SOFT<RGB12>::send_rgb(RGB12 color)
{
  uint16_t rgb = color.rgb;

  for (uint16_t mask = 0x8000; mask; mask >>= 1) {
    if (rgb & mask) ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
  }
}

template<>
void ST7735_SOFT<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  uint16_t rgb = color.rgb;

  ST_SOFT_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
#ifdef __AVR__   
  reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
  reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
  reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);

  while (len--) {
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
  ST_SOFT_SCK(CLR);
#else

  while (len--) {
    for (int mask = 0x800; mask; mask >>= 1) {
      if (rgb & mask) ST_SOFT_SDA(SET);
      else ST_SOFT_SDA(CLR);
      ST_SOFT_SCK(SET);
      ST_SOFT_SCK(CLR);
    }
  }
#endif
  release();
}
