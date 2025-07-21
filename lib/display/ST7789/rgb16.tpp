template<>
ATTR_INLINE void ST7789<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x55); // 5x6x5 bit
}

template<>
ATTR_INLINE void ST7789<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
ATTR_NOINLINE void ST7789<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);

#ifdef MIK32V2
  uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
  while (len--) send_word(color.rgb);
#else
  uint16_t x = x1 - x0;
  uint16_t y = y1 - y0;
  for (uint16_t i = 0; i <= x; i++)
    for (uint16_t j = 0; j <= y; j++) {
      send_word(color.rgb);
    }
#endif
  release();
}
