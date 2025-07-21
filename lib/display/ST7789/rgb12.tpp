template<>
ATTR_INLINE void ST7789<RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x53); // 4x4x4 bit
}

template<>
ATTR_INLINE void ST7789<RGB12>::select() { ST_8_CS(CLR); flag = 0; }

template<>
ATTR_INLINE void ST7789<RGB12>::send_rgb(RGB12 color)
{
  static uint8_t half;

#ifdef MIK32V2
  if (flag) {
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = half | (color.rgb >> 8);
    ST_8_WR(SET);
    flag = 0;
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = color.rgb & 0xff;
    ST_8_WR(SET);
  }
  else {
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = color.rgb >> 4;
    half = color.rgb << 4;
    flag = 1;
    ST_8_WR(SET);
  }
#else 
  if (flag) {
    ST_8_PORT(OUTPUT) = half | (color.rgb >> 8);
    ST_8_WR(SET); ST_8_WR(CLR);
    flag = 0;
    ST_8_PORT(OUTPUT) = color.rgb;
    ST_8_WR(SET); ST_8_WR(CLR);
  }
  else {
    ST_8_PORT(OUTPUT) = color.rgb >> 4;
    ST_8_WR(SET); ST_8_WR(CLR);
    half = color.rgb << 4;
    flag = 1;
  }
#endif
}

template<>
ATTR_INLINE void ST7789<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  select();
  set_addr(x0, y0, x1, y1);

  uint8_t hbyte = color.rgb >> 4;
  uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
  uint8_t lbyte = color.rgb;

#ifdef MIK32V2
  uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;

  while (len--) {
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = hbyte;
    ST_8_WR(SET);
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = mbyte;
    ST_8_WR(SET);
    ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
    ST_8_PORT(STATE) = lbyte;
    ST_8_WR(SET);
  }
#else
  uint16_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
  while (len--) {
    ST_8_PORT(OUTPUT) = hbyte;
    ST_8_WR(SET); ST_8_WR(CLR);
    ST_8_PORT(OUTPUT) = mbyte;
    ST_8_WR(SET); ST_8_WR(CLR);
    ST_8_PORT(OUTPUT) = lbyte;
    ST_8_WR(SET); ST_8_WR(CLR);
  }
#endif
  release();
}
