////////////////////////////// SPI 0 //////////////////////////////////////////

template<>
void ST7735<SPI0, RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
  spi_conf.thr(SPI_THR_3);
}

template<>
ATTR_INLINE  void ST7735<SPI0, RGB12>::send_rgb(RGB12 color)
{
  static uint8_t half;

  if (flag) {
    spi.wait_thr();
    spi.send(half | (color.rgb >> 8));
    flag = 0;
    spi.wait();
    spi.send(color.rgb);
  }
  else {
    spi.send(color.rgb >> 4);
    half = color.rgb << 4;
    flag = 1;
  }
}

template<>
void ST7735<SPI0, RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;

  uint8_t hbyte = color.rgb >> 4;
  uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
  uint8_t lbyte = color.rgb;

  while (len--) {
    spi.wait_thr();
    spi.send(hbyte);
    spi.wait();
    spi.send(mbyte);
    spi.wait();
    spi.send(lbyte);
  }
  release();
}

////////////////////////////// SPI 1 //////////////////////////////////////////

template<>
void ST7735<SPI1, RGB12>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
  spi_conf.thr(SPI_THR_3);
}

template<>
ATTR_INLINE void ST7735<SPI1, RGB12>::send_rgb(RGB12 color)
{
  static uint8_t half;

  if (flag) {
    spi.wait_thr();
    spi.send(half | (color.rgb >> 8));
    flag = 0;
    spi.wait();
    spi.send(color.rgb);
  }
  else {
    spi.send(color.rgb >> 4);
    half = color.rgb << 4;
    flag = 1;
  }
}

template<>
void ST7735<SPI1, RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
{
  select();
  set_addr(x0, y0, x1, y1);
  uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1)) >> 1;

  uint8_t hbyte = color.rgb >> 4;
  uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
  uint8_t lbyte = color.rgb;

  while (len--) {
    spi.wait_thr();
    spi.send(hbyte);
    spi.wait();
    spi.send(mbyte);
    spi.wait();
    spi.send(lbyte);
  }
  release();
}
