template<>
ATTR_INLINE void ST7789<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}

template<>
ATTR_INLINE void ST7789<RGB18>::send_rgb(RGB18 color)
{
#ifdef MIK32V2
  ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
  ST_8_PORT(STATE) = color.red;
  ST_8_WR(SET);
  ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
  ST_8_PORT(STATE) = color.green;
  ST_8_WR(SET);
  ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
  ST_8_PORT(STATE) = color.blue;
  ST_8_WR(SET);
#else
  ST_8_PORT(OUTPUT) = color.red;
  ST_8_WR(SET); ST_8_WR(CLR);
  ST_8_PORT(OUTPUT) = color.green;
  ST_8_WR(SET); ST_8_WR(CLR);
  ST_8_PORT(OUTPUT) = color.blue;
  ST_8_WR(SET); ST_8_WR(CLR);
#endif
}

template<>
ATTR_INLINE void ST7789<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);

#ifdef MIK32V2
  uint32_t red = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.red;
  uint32_t green = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.green;
  uint32_t blue = (ST_8_PORT(OUTPUT) & ~(0xff | ST_8_WR(MASK))) | color.blue;
  uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) {
    ST_8_PORT(OUTPUT) = red;
    ST_8_WR(SET);
    ST_8_PORT(OUTPUT) = green;
    ST_8_WR(SET);
    ST_8_PORT(OUTPUT) = blue;
    ST_8_WR(SET);

    // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    // ST_8_PORT(STATE) = color.red;
    // ST_8_WR(SET);
    // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    // ST_8_PORT(STATE) = color.green;
    // ST_8_WR(SET);
    // ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    // ST_8_PORT(STATE) = color.blue;
    // ST_8_WR(SET);
  }
#else
  for (uint16_t i = y0; i <= y1; i++)
    for (uint16_t j = x0; j <= x1; j++) {
      ST_8_PORT(OUTPUT) = color.red;
      ST_8_WR(SET); ST_8_WR(CLR);
      ST_8_PORT(OUTPUT) = color.green;
      ST_8_WR(SET); ST_8_WR(CLR);
      ST_8_PORT(OUTPUT) = color.blue;
      ST_8_WR(SET); ST_8_WR(CLR);
    }
#endif
  release();
}
