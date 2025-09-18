// Разрешение дисплея
INLINE constexpr int16_t max_x() { return 239; }
INLINE constexpr int16_t max_y() { return 319; }
INLINE  void release() { ST_8_CS(SET); }
INLINE void send_rgb(RGB color, int32_t len) { while (len--) send_rgb(color); }

void init(uint8_t rotation = 0)
{
  ST_8_RD(GPIO); ST_8_WR(GPIO); ST_8_RS(GPIO); ST_8_CS(GPIO); ST_8_RST(GPIO);
  ST_8_RD(OUT); ST_8_WR(OUT); ST_8_RS(OUT); ST_8_CS(OUT); ST_8_RST(OUT);
  ST_8_PORT(OUT) | 0xFF;
  ST_8_RD(SET); ST_8_WR(CLR); ST_8_RS(CLR); ST_8_CS(SET);
  ST_8_RST(CLR);
  ST_8_RST(SET);

  select();
  delay_ms(1);
  send_command(SWRESET);
  delay_ms(25);

  send_config(ST7789_CONFIG, sizeof(ST7789_CONFIG));
  send_command(MADCTL); send_byte(rotation);
  set_rgb_format();
  send_command(SLPOUT);
  send_command(NORON);
  send_command(DISPON);
  release();
}

INLINE void send_command(uint8_t command)
{
  ST_8_RS(CLR);
  send_byte(command);
  ST_8_RS(SET);
}

INLINE void send_byte(uint8_t data)
{
#ifdef MIK32V2
  ST_8_PORT(CLR) | 0xff;
  ST_8_PORT(STATE) = data;
  ST_8_WR(CLR);
  ST_8_WR(SET);
#else
  ST_8_PORT(OUTPUT) = data;
  ST_8_WR(SET); ST_8_WR(CLR);
#endif
}

INLINE void send_word(uint16_t data)
{
#ifdef MIK32V2
  ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
  ST_8_PORT(STATE) = data >> 8;
  ST_8_WR(SET);
  ST_8_PORT(CLR) | 0xff | ST_8_WR(MASK);
  ST_8_PORT(STATE) = data & 0xff;
  ST_8_WR(SET);
#else
  ST_8_PORT(OUTPUT) = data >> 8;
  ST_8_WR(SET); ST_8_WR(CLR);
  ST_8_PORT(OUTPUT) = data;
  ST_8_WR(SET); ST_8_WR(CLR);
#endif
}

void pixel(int16_t x, int16_t y, RGB color)
{
  select();
  set_addr(x, y, x, y);
  send_rgb(color);
  send_rgb(color);
  release();
}
