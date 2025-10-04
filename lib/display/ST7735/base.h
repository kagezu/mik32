
protected:
  INLINE void send_command(uint8_t command)
  {
    ST_SPI_RS.clr();  // Запись команды
    send_byte(command);
    ST_SPI_RS.set();  // Запись данных
  }

public:
  S spi;

  INLINE constexpr int16_t max_x() { return 127; }
  INLINE constexpr int16_t max_y() { return 159; }
  INLINE void release()
  {
    spi.end();
    ST_SPI_CS.set();
  }
  INLINE void send_rgb(RGB color, uint8_t len)
  {
    while (len--) send_rgb(color);
  }
  INLINE void send_byte(uint8_t data)
  {
    spi.send(data);
    spi.wait_idle();
  }
  INLINE void send_word(uint16_t data)
  {
    spi.send16(data);
    spi.wait_idle();
  }

  void init(uint8_t rotation = 0)
  {
    ST_SPI_RST.init(GPO_Max);
    ST_SPI_RST.clr();
    ST_SPI_CS.init(GPO_Max);
    ST_SPI_CS.set();
    ST_SPI_RS.init(GPO_Max);
    ST_SPI_RS.set();
    ST_SPI_RST.set();

    delay_ms(50);

    // Настройка для инициализации
    spi.mode(SPI_MODE0);
    spi.fq(4000);

    select();
    send_command(SWRESET);
    delay_ms(50);
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(rotation);
    set_rgb_format();
    send_command(DISPON);
    release();

    spi.fq(16000);        // По даташиту
  #ifdef MIK32V2
    spi.mode(SPI_MODE3);  // Ускоряет ещё на 11,7% для MIK32
  #endif
  #ifdef CH32V20x_D6
    spi.fq(36000);  // Максимум
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
