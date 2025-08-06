#include "driver.h"
// #ifdef __AVR_ATmega328P__

void SSD1306::init(uint8_t rotation)
{
  i2c.init();
  i2c.set_freq(0);
  i2c.set_address(LCD_I2C_ADDR);

  delay_ms(10);
  sei();
  send_config(SSD1306_CONFIG, sizeof(SSD1306_CONFIG));
  if (rotation & FLIP_X) send_command(SetSegmentMapFlip);
  if (rotation & FLIP_Y) send_command(SetScanDirectionFlip);
  if (rotation & FLIP_Y) send_command(SetPinsConfig, 0x22);
  ex = rotation & EX_X_Y;
  send_command(SetDisplayON);
  update();
}

void SSD1306::send_config(const uint8_t *command, uint8_t count)
{
  i2c.begin();
  i2c.write(0);
  while (count--) i2c.write(pgm_read_byte(command++));
  i2c.end();
}

void SSD1306::send_command(uint8_t command)
{
  i2c.begin();
  i2c.write(0);
  i2c.write(command);
  i2c.end();
}

void SSD1306::send_command(uint8_t command, uint8_t arg)
{
  i2c.begin();
  i2c.write(0);
  i2c.write(command);
  i2c.write(arg);
  i2c.end();
}

void SSD1306::send_command(uint8_t command, uint8_t arg1, uint8_t arg2)
{
  i2c.begin();
  i2c.write(0);
  i2c.write(command);
  i2c.write(arg1);
  i2c.write(arg2);
  i2c.end();
}

void SSD1306::set_addr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  begin_x = x0;
  begin_y = y0;
  end_x = x1;
  end_y = y1;
  curent_x = x0;
  curent_y = y0;
}

void SSD1306::send_rgb(bool color)
{
  pixel(curent_x, curent_y, color);

  curent_x++;
  if (curent_x > end_x) {
    curent_x = begin_x;
    curent_y++;
    if (curent_y > end_y) curent_y = begin_y;
  }
}

void SSD1306::pixel(uint8_t x, uint8_t y, bool color)
{
  if (ex) { uint8_t t = x; x = y; y = t; }
  if (x > max_x() || y > max_y()) return;
  uint8_t *pixel = &buffer[x + (y >> 3) * (max_x() + 1)];
  uint8_t bit = (1 << (y & 7));
  if (color)*pixel |= bit;
  else *pixel &= ~bit;
}

void SSD1306::area(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, bool color)
{
  for (uint8_t xx = x; xx <= x1; xx++)
    for (uint8_t yy = x; yy <= y1; yy++)
      pixel(xx, yy, color);
}

void SSD1306::update(void)
{

  send_command(SetPageAddress, 0, 0xFF);
  send_command(SetColumnAddress, 0, max_x());
  i2c.begin();
  i2c.write(0x40);
  i2c.write(buffer, sizeof(buffer));
  i2c.end();
}

// #endif
