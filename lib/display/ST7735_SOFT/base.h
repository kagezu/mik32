INLINE constexpr int16_t max_x() { return 127; }
INLINE constexpr int16_t max_y() { return 159; }
INLINE void select() { ST_SOFT_CS.clr(); }
INLINE void release() { ST_SOFT_SCK.clr(); ST_SOFT_CS.set(); }
INLINE void send_rgb(RGB color, uint8_t len) { while (len--)send_rgb(color); }

void pixel(int16_t x, int16_t y, RGB color)
{
  select();
  set_addr(x, y, x, y);
  send_rgb(color);
  send_rgb(color);
  release();
}

void init(uint8_t rotation = 0)
{
  ST_SOFT_SCK.init(GPO_2mA);
  ST_SOFT_SDA.init(GPO_2mA);
  ST_SOFT_RST.init(GPO_2mA);
  ST_SOFT_CS.init(GPO_2mA);
  ST_SOFT_RS.init(GPO_2mA);
  ST_SOFT_CS.set();
  ST_SOFT_RS.set();
  ST_SOFT_SCK.clr();

  ST_SOFT_RST.clr();    // Аппаратный сброс
  delay_ms(2);
  ST_SOFT_RST.set();
  delay_ms(30);         // Ждать стабилизации напряжений

  select();
  send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
  send_command(MADCTL);
  send_byte(rotation);
  set_rgb_format();
  send_command(DISPON);
  release();
}

void send_command(uint8_t command)
{
  ST_SOFT_RS.clr(); // Запись команды
  send_byte(command);
  ST_SOFT_RS.set(); // Запись данных
}

void send_byte(uint8_t data)
{
  for (uint8_t mask = 0x80; mask; mask >>= 1) {
    if (data & mask) ST_SOFT_SDA.set();
    else ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
  }
}

void send_word(uint16_t rgb)
{
  u16 mask = 0x8000;
  while (mask) {
    if (rgb & mask)  ST_SOFT_SDA.set();
    else  ST_SOFT_SDA.clr();
    ST_SOFT_SCK.set();
    ST_SOFT_SCK.clr();
    mask >>= 1;
  }
}
