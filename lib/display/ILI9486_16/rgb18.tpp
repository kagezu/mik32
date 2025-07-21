template<>
ATTR_INLINE void ILI9486_16<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x66); // 6x6x6 bit (24 bit transfer)
}

template<>
ATTR_INLINE void ILI9486_16<RGB18>::select() { L_CS(CLR); flag = 0; }

template<>
void ILI9486_16<RGB18>::send_rgb(RGB18 color)
{
  static uint16_t half;
  if (flag) {
    L_PORT(OUTPUT) = color.red | half;
    L_WR(SET); L_WR(CLR);
    flag = 0;
    L_PORT(OUTPUT) = color.rgb;
    L_WR(SET); L_WR(CLR);
  }
  else {
    L_PORT(OUTPUT) = color.rgb >> 8;
    L_WR(SET); L_WR(CLR);
    half = color.blue << 8;
    flag = 1;
  }
}

template<>
void ILI9486_16<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
  uint16_t hword = *(uint16_t *)&color.green;
  uint16_t mword = color.red | (color.blue << 8);
  uint16_t lword = *(uint16_t *)&color.blue;

  while (len--) {
    L_PORT(OUTPUT) = hword;
    L_WR(SET);
    L_WR(CLR);

    L_PORT(OUTPUT) = mword;
    L_WR(SET);
    L_WR(CLR);

    L_PORT(OUTPUT) = lword;
    L_WR(SET);
    L_WR(CLR);
  }
  release();
}
