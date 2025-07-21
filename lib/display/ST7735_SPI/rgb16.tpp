////////////////////////////// SPI 0 //////////////////////////////////////////

template<>
void ST7735<SPI0, RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
  spi_conf.thr(SPI_THR_2);
}

template<>
ATTR_INLINE void ST7735<SPI0, RGB16>::send_rgb(RGB16 color)
{
  spi.wait_thr();
  spi.send16(color.rgb);
}

template<>
void ST7735<SPI0, RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) { spi.wait_thr();  spi.send16(color.rgb); }
  release();
}

////////////////////////////// SPI 1 //////////////////////////////////////////

template<>
void ST7735<SPI1, RGB16>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
  spi_conf.thr(SPI_THR_2);
}

template<>
ATTR_INLINE void ST7735<SPI1, RGB16>::send_rgb(RGB16 color)
{
  spi.wait_thr();
  spi.send16(color.rgb);
}

template<>
void ST7735<SPI1, RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) { spi.wait_thr();  spi.send16(color.rgb); }
  release();
}
