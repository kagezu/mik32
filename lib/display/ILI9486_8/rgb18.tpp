template<>
ATTR_INLINE void ILI9486_8<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}

template<>
ATTR_INLINE void ILI9486_8<RGB18>::send_rgb(RGB18 color)
{
#ifdef MIK32V2
  uint32_t mask = ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK));
  ILI_8_PORT(OUTPUT) = color.red | mask;
  ILI_8_WR(SET);
  ILI_8_PORT(OUTPUT) = color.green | mask;
  ILI_8_WR(SET);
  ILI_8_PORT(OUTPUT) = color.blue | mask;
  ILI_8_WR(SET);
#else
  ILI_8_PORT(OUTPUT) = color.red;
  ILI_8_WR(SET); ILI_8_WR(CLR);
  ILI_8_PORT(OUTPUT) = color.green;
  ILI_8_WR(SET); ILI_8_WR(CLR);
  ILI_8_PORT(OUTPUT) = color.blue;
  ILI_8_WR(SET); ILI_8_WR(CLR);
#endif
}

template<>
ATTR_INLINE void ILI9486_8<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);

#ifdef MIK32V2
  uint32_t red = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.red;
  uint32_t green = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.green;
  uint32_t blue = (ILI_8_PORT(OUTPUT) & ~(0xff | ILI_8_WR(MASK))) | color.blue;
  uint32_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) {
    ILI_8_PORT(OUTPUT) = red;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = green;
    ILI_8_WR(SET);
    ILI_8_PORT(OUTPUT) = blue;
    ILI_8_WR(SET);
  #else
  RGB32 rgb = color.rgb32();
  uint16_t x = x1 - x0;
  uint16_t y = y1 - y0;
  for (uint16_t i = 0; i <= x; i++)
    for (uint16_t j = 0; j <= y; j++) {
      ILI_8_PORT(OUTPUT) = rgb.red;
      ILI_8_WR(SET); ILI_8_WR(CLR);
      ILI_8_PORT(OUTPUT) = rgb.green;
      ILI_8_WR(SET); ILI_8_WR(CLR);
      ILI_8_PORT(OUTPUT) = rgb.blue;
      ILI_8_WR(SET); ILI_8_WR(CLR);
    #endif
  }
  release();
}
