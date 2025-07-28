template<>
ATTR_INLINE void ILI9486_8<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
}

template<>
ATTR_INLINE void ILI9486_8<RGB16>::send_rgb(RGB16 color)
{
  send_word(color.rgb);
}

template<>
void ILI9486_8<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint8_t h = color.rgb >> 8;
  uint8_t l = color.rgb & 0xff;

#ifdef MIK32V2
  uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
  while (len--) {
    ILI_8_PORT(CLR) | 0xff | ILI_8_WR(MASK);
    ILI_8_PORT(STATE) = h;
    ILI_8_WR(SET);
    ILI_8_PORT(CLR) | 0xff | ILI_8_WR(MASK);
    ILI_8_PORT(STATE) = l;
    ILI_8_WR(SET);
  }
#else
  uint16_t x = x1 - x0;
  uint16_t y = y1 - y0;
  for (uint16_t i = 0; i <= x; i++)
    for (uint16_t j = 0; j <= y; j++) {
      ILI_8_PORT(OUTPUT) = h;
      ILI_8_WR(SET); ILI_8_WR(CLR);
      ILI_8_PORT(OUTPUT) = l;
      ILI_8_WR(SET); ILI_8_WR(CLR);
    }
#endif
  release();
}
