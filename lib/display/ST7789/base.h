// Разрешение дисплея
INLINE constexpr int16_t max_x() { return 239; }
INLINE constexpr int16_t max_y() { return 319; }
INLINE  void release() { ST_8_CS.set(); }
INLINE void send_rgb(RGB color, int32_t len) { while (len--) send_rgb(color); }

void init(uint8_t rotation = 0)
{
#ifdef MIK32V2
  ST_8_RD.init(GPO_Max); ST_8_WR.init(GPO_Max); ST_8_RS.init(GPO_Max); ST_8_CS.init(GPO_Max); ST_8_RST.init(GPO_Max);
  ST_8_PORT.init(GPO_Max);
  ST_8_RD.set(); ST_8_WR.clr(); ST_8_RS.clr(); ST_8_CS.set();
  ST_8_RST.clr();
  ST_8_RST.set();
#else

#endif

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
  ST_8_RS.clr();
  send_byte(command);
  ST_8_RS.set();
}

INLINE void send_byte(uint8_t data)
{
  ST_8_PORT.out(data);
  ST_8_WR.set(); ST_8_WR.clr();
}

INLINE void send_word(uint16_t data)
{
  ST_8_PORT.out(data >> 8);
  ST_8_WR.set(); ST_8_WR.clr();
  ST_8_PORT.out(data);
  ST_8_WR.set(); ST_8_WR.clr();
}

void pixel(int16_t x, int16_t y, RGB color)
{
  select();
  set_addr(x, y, x, y);
  send_rgb(color);
  send_rgb(color);
  release();
}
