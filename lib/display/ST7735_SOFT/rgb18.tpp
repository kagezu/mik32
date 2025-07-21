template<>
void ST7735_SOFT<RGB18>::set_rgb_format()
{
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
}

template<>
void ST7735_SOFT<RGB18>::send_rgb(RGB18 color)
{
  uint8_t r = color.red;
  uint8_t g = color.green;
  uint8_t b = color.blue;

  if (r & 0x80)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (r & 0x40)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (r & 0x20)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (r & 0x10)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (r & 0x8)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (r & 0x4)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);

  if (g & 0x80)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (g & 0x40)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (g & 0x20)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (g & 0x10)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (g & 0x8)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (g & 0x4)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);

  if (b & 0x80)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (b & 0x40)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (b & 0x20)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (b & 0x10)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (b & 0x8)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  if (b & 0x4)  ST_SOFT_SDA(SET);
  else ST_SOFT_SDA(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
  ST_SOFT_SCK(SET);
  ST_SOFT_SCK(CLR);
}

template<>
void ST7735_SOFT<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
{
  uint8_t r = color.red;
  uint8_t g = color.green;
  uint8_t b = color.blue;

  ST_SOFT_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
  while (len--) {
    if (r & 0x80)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (r & 0x40)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (r & 0x20)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (r & 0x10)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (r & 0x8)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (r & 0x4)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);

    if (g & 0x80)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (g & 0x40)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (g & 0x20)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (g & 0x10)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (g & 0x8)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (g & 0x4)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);

    if (b & 0x80)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (b & 0x40)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (b & 0x20)  ST_SOFT_SDA(SET);
    else ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (b & 0x10)  ST_SOFT_SDA(SET);
    else  ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (b & 0x8)  ST_SOFT_SDA(SET);
    else  ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    if (b & 0x4)  ST_SOFT_SDA(SET);
    else  ST_SOFT_SDA(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
    ST_SOFT_SCK(SET);
    ST_SOFT_SCK(CLR);
  }
  ST_SOFT_CS(SET);
}
