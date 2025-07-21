template<>
void ILI9486_16<RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x55); // 5x6x5 bit
}

template<>
ATTR_INLINE void ILI9486_16<RGB16>::send_rgb(RGB16 color)
{
  L_PORT(OUTPUT) = color.rgb;
  L_WR(SET);
  L_WR(CLR);
}

template<>
ATTR_INLINE void ILI9486_16<RGB16>::send_rgb(RGB16 color, int32_t len)
{
  L_PORT(OUTPUT) = color.rgb;
  while (len--) {
    L_WR(SET);
    L_WR(CLR);
  }
}

template<>
void ILI9486_16<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);

  uint32_t len = (x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1);
  L_PORT(OUTPUT) = color.rgb;
  while (len--) {
    L_WR(SET);
    L_WR(CLR);
  }
  release();
}
