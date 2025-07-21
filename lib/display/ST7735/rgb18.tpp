////////////////////////////// SPI 0 //////////////////////////////////////////

template<>
void ST7735<SPI0, RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  spi_conf.thr(SPI_THR_3);
}

template<>
ATTR_INLINE void ST7735<SPI0, RGB18>::send_rgb(RGB18 color)
{
  spi.wait_thr();
  spi.send(color.red);
  spi.wait();
  spi.send(color.green);
  spi.wait();
  spi.send(color.blue);
}

template<>
void ST7735<SPI0, RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) {
    spi.wait_thr();
    spi.send(color.red);
    spi.wait();
    spi.send(color.green);
    spi.wait();
    spi.send(color.blue);
  }
  release();
}

////////////////////////////// SPI 1 //////////////////////////////////////////

template<>
void ST7735<SPI1, RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  spi_conf.thr(SPI_THR_3);
}

template<>
ATTR_INLINE void ST7735<SPI1, RGB18>::send_rgb(RGB18 color)
{
  spi.wait_thr();
  spi.send(color.red);
  spi.wait();
  spi.send(color.green);
  spi.wait();
  spi.send(color.blue);
}

template<>
void ST7735<SPI1, RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  while (len--) {
    spi.wait_thr();
    spi.send(color.red);
    spi.wait();
    spi.send(color.green);
    spi.wait();
    spi.send(color.blue);
  }
  release();
}
