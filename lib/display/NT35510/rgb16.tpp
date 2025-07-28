template<>
void NT35510<RGB16>::set_rgb_format()
{
  send_command(NT_COLMOD);
  send_byte(0x05); // 5x6x5 bit
}

template<>
ATTR_INLINE void NT35510<RGB16>::send_rgb(RGB16 color)
{
  NT_PORT(OUTPUT) = color.rgb;
  NT_WR(CLR);
  NT_WR(SET);
}

template<>
ATTR_INLINE void NT35510<RGB16>::send_rgb(RGB16 color, int32_t len)
{
  NT_PORT(OUTPUT) = color.rgb;
  while (len--) {
    NT_WR(CLR);
    NT_WR(SET);
  }
}

template<>
void NT35510<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);

  uint32_t len = (x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1);
  NT_PORT(OUTPUT) = color.rgb;
  // while (len--) {
  //   NT_WR(CLR);
  //   NT_WR(SET);
  // }

  len <<= 1;
  T32_1_TOP(0);
  NT_WR(TIMER);
  TIMER32_2->CHANNELS[0].CNTRL =
    TIMER32_CH_CNTRL_MODE_PWM_M |
    TIMER32_CH_CNTRL_ENABLE_M;
  T32_1_C;
  while (T32_1 < len);// NT_PORT(OUTPUT) = T32_1 * color.rgb;
  TIMER32_2->CHANNELS[0].CNTRL = 0;
  NT_WR(GPIO);

  release();
}
