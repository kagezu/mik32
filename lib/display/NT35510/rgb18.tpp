template<>

ATTR_INLINE void send_rgb(RGB color, int32_t len) { while (len--) send_rgb(color); }
ATTR_INLINE void NT35510<RGB18>::set_rgb_format()
{
  send_command(NT_COLMOD);
  send_byte(0x06); // 6x6x6 bit
}

template<>
void NT35510<RGB18>::send_rgb(RGB18 color)
{
  static uint16_t half;
  if (flag) {
    NT_PORT(OUTPUT) = color.red | half;
    NT_WR(CLR); NT_WR(SET);
    flag = 0;
    NT_PORT(OUTPUT) = color.rgb;
    NT_WR(CLR); NT_WR(SET);
  }
  else {
    NT_PORT(OUTPUT) = (color.rgb >> 8);
    NT_WR(CLR); NT_WR(SET);
    half = color.blue << 8;
    flag = 1;
  }
}

template<>
void NT35510<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
  uint16_t hword = *(uint16_t *)&color.green;
  uint16_t mword = color.red | (color.blue << 8);
  uint16_t lword = *(uint16_t *)&color.blue;

  while (len--) {
    NT_PORT(OUTPUT) = hword;
    NT_WR(CLR);
    NT_WR(SET);

    NT_PORT(OUTPUT) = mword;
    NT_WR(CLR);
    NT_WR(SET);

    NT_PORT(OUTPUT) = lword;
    NT_WR(CLR);
    NT_WR(SET);
  }
  release();
}
