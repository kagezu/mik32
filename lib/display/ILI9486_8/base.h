INLINE constexpr int16_t max_x() { return 319; }
INLINE constexpr int16_t max_y() { return 479; }
INLINE  void select() { ILI_8_CS.clr(); }
INLINE  void release() { ILI_8_CS.set(); }
INLINE void send_rgb(RGB color, int32_t len) { while (len--) send_rgb(color); }

void init(uint8_t rotation = 0)
{

  ILI_8_RD.init(GPO_2mA);
  ILI_8_WR.init(GPO_2mA);
  ILI_8_RS.init(GPO_2mA);
  ILI_8_CS.init(GPO_2mA);
  ILI_8_RST.init(GPO_2mA);
  ILI_8_PORT.init(GPO_2mA);

  ILI_8_RST.clr();
  ILI_8_RD.set();
  ILI_8_WR.set();
  ILI_8_RS.set();
  ILI_8_CS.set();
  ILI_8_RST.set();

  select();
  send_command(SLPOUT);
  delay_ms(50);
  send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
  send_command(MADCTL); send_byte((rotation | 0x08));// BGR -> RBG
  set_rgb_format();
  delay_ms(50);
  send_command(NORON);
  send_command(DISPON);
  release();
}

INLINE void scroll(uint16_t sl)
{
  select();
  send_command(VSCRSADD);
  send_word(sl);
  release();
}

INLINE void send_command(uint8_t command)
{
  ILI_8_RS.clr();
  send_byte(command);
  ILI_8_RS.set();
}

INLINE void send_byte(uint8_t data)
{
#ifdef MIK32V2
  ILI_8_PORT.clr(ILI_8_MASK);
  ILI_8_PORT.set(data);
  ILI_8_WR.set();
#else
  ILI_8_PORT(OUTPUT) = data;
  ILI_8_WR(SET); ILI_8_WR(CLR);
#endif
}

INLINE void send_word(uint16_t data)
{
#ifdef MIK32V2
  ILI_8_PORT.clr(ILI_8_MASK);
  ILI_8_PORT.set(data >> 8);
  ILI_8_WR.set();
  ILI_8_PORT.clr(ILI_8_MASK);
  ILI_8_PORT.set(data & 0xff);
  ILI_8_WR.set();
#else
  ILI_8_PORT(OUTPUT) = data >> 8;
  ILI_8_WR(SET); ILI_8_WR(CLR);
  ILI_8_PORT(OUTPUT) = data;
  ILI_8_WR(SET); ILI_8_WR(CLR);
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
