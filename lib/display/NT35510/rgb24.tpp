template<>
ATTR_INLINE void NT35510<RGB24>::set_rgb_format()
{
  send_command(NT_COLMOD);
  send_byte(0x07); // 8x8x8 bit
}

template<>
void NT35510<RGB24>::send_rgb(RGB24 color)
{
  static uint16_t half;
  if (flag) {
    L_PORT(OUTPUT) = color.red | half;
    L_WR(CLR); L_WR(SET);
    flag = 0;
    L_PORT(OUTPUT) = color.rgb;
    L_WR(CLR); L_WR(SET);
  }
  else {
    L_PORT(OUTPUT) = (color.rgb >> 8);
    L_WR(CLR); L_WR(SET);
    half = color.blue << 8;
    flag = 1;
  }
}

template<>
void NT35510<RGB24>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB24 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
  uint16_t hword = *(uint16_t *)&color.green;
  uint16_t mword = color.red | (color.blue << 8);
  uint16_t lword = *(uint16_t *)&color.blue;

  while (len--) {
    L_PORT(OUTPUT) = hword;
    L_WR(CLR);
    L_WR(SET);

    L_PORT(OUTPUT) = mword;
    L_WR(CLR);
    L_WR(SET);

    L_PORT(OUTPUT) = lword;
    L_WR(CLR);
    L_WR(SET);
  }
  release();
}
