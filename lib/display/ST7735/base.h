
protected:
  ATTR_INLINE void send_command(uint8_t command)
  {
    ST_SPI_RS(CLR); // Запись команды
    send_byte(command);
    ST_SPI_RS(SET); // Запись данных
  }

public:
  S spi;
  SPIConf spi_conf;

  ATTR_INLINE constexpr int16_t max_x() { return 127; }
  ATTR_INLINE constexpr int16_t max_y() { return 159; }
  ATTR_INLINE void release() { spi.end(); ST_SPI_CS(SET); }
  ATTR_INLINE void send_rgb(RGB color, uint8_t len) { while (len--)send_rgb(color); }
  ATTR_INLINE void send_byte(uint8_t data) { spi.send(data); spi.wait_idle(); }
  ATTR_INLINE void send_word(uint16_t data) { spi.send16(data); spi.wait_idle(); }

  void init(uint8_t rotation = 0)
  {
    ST_SPI_RST(OUT); ST_SPI_RST(CLR);
    ST_SPI_CS(OUT); ST_SPI_CS(SET);
    ST_SPI_RS(OUT); ST_SPI_RS(SET);
    ST_SPI_RST(SET);

    // Настройка для инициализации
    spi_conf.mode(SPI_MODE0);

    select();
    send_command(SWRESET);
    delay_ms(50);
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(rotation);
    set_rgb_format();
    send_command(DISPON);
    release();

    spi_conf.mode(SPI_MODE3);// Ускоряет ещё на 11,7% для MIK32
  }

  void pixel(int16_t x, int16_t y, RGB color)
  {
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }
