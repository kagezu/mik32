ATTR_INLINE constexpr int16_t max_x() { return 479; }
ATTR_INLINE constexpr int16_t max_y() { return 799; }

void init(uint8_t rotation = 0) {
  GPIO_NT35510();
  select();
  send_command(NT_SWRESET);
  send_config(NT35510_CONFIG_STEP0, sizeof(NT35510_CONFIG_STEP0));
  delay_ms(50);
  send_config(NT35510_CONFIG_STEP1, sizeof(NT35510_CONFIG_STEP1));
  send_command(NT_MADCTL);
  send_byte((rotation | 0x00));  // BGR -> RBG
  set_rgb_format();
  send_command(NT_NORON);
  send_command(NT_SLPOUT);
  delay_ms(30);
  send_command(NT_DISPON);
  release();
}


ATTR_INLINE void send_command(uint16_t com) {
  NT_PORT(OUTPUT) = com;
  NT_RS(CLR);
  NT_WR(SET);
  NT_WR(CLR);
  NT_WR(SET);
  NT_WR(CLR);
  NT_RS(SET);
}

ATTR_INLINE void send_byte(uint8_t data) {

  NT_PORT(OUTPUT) = data;
  NT_WR(SET);
  NT_WR(CLR);
}

ATTR_INLINE void send_word(uint16_t data) {
  // NT_WR(CLR);
  NT_PORT(OUTPUT) = (data >> 8);
  NT_WR(CLR);
  NT_WR(SET);
  NT_PORT(OUTPUT) = data;
  NT_WR(CLR);
  NT_WR(SET);
}

void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  send_command(NT_CASET);
  send_byte(x0 >> 8);
  send_command(NT_CASET + 1);
  send_byte(x0);
  send_command(NT_CASET + 2);
  send_byte(x1 >> 8);
  send_command(NT_CASET + 3);
  send_byte(x1);
  send_command(NT_RASET);
  send_byte(y0 >> 8);
  send_command(NT_RASET + 1);
  send_byte(y0);
  send_command(NT_RASET + 2);
  send_byte(y1 >> 8);
  send_command(NT_RASET + 3);
  send_byte(y1);
  send_command(NT_RAMWR);
}

void send_config(const uint8_t *config, int16_t size) {
  while (size > 0) {
    uint8_t count = *config++;
    uint16_t comand = (uint16_t)(*config++) << 8;
    size -= 2 + count;
    for (int i = 0; i < count; i++) {
      send_command(comand + 1);
      send_byte(*config++);
    }
  }
}

void pixel(int16_t x, int16_t y, RGB color) {
  select();
  set_addr(x, y, x, y);
  send_rgb(color);
  send_rgb(color);
  release();
}
