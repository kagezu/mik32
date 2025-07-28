#include "driver.h"
#ifdef __AVR_ATmega328P__

void SSD1306::init(uint8_t rotation)
{
  i2c.init();
  i2c.set_freq(0);
  i2c.set_address(LCD_I2C_ADDR);

  sei();
  send_command(SetDisplayOFF);
  send_config(SSD1306_CONFIG, sizeof(SSD1306_CONFIG));
  send_command(SetMultiplexRatio, max_y());
  send_command(SetPrechargePeriod, 0xF1);

  send_command(SetPinsConfig, 0x02);
  // send_command(SetPinsConfig, 0x22);

  send_command(SetDisplayON);
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

void SSD1306::set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  begin_x = x0;
  begin_y = y0;
  end_x = x1;
  end_y = y1;
  curent_x = 0;
  curent_y = 0;
}

void SSD1306::send_rgb(RGB color)
{
  uint8_t *pixel = &buffer[curent_x + (curent_y >> 3) * (max_x() + 1)];
  uint8_t bit = (1 << (curent_y & 7));
  *pixel &= ~bit;
  *pixel |= color ? bit : 0;

  curent_x++;
  if (curent_x > end_x) {
    curent_x = begin_x;
    curent_y++;
    if (curent_y > end_y) curent_y = begin_y;
  }
}

void SSD1306::pixel(uint8_t x, uint8_t y, RGB color)
{
  if (x > max_x() || y > max_y()) return;
  uint8_t *pixel = &buffer[x + (y >> 3) * (max_x() + 1)];
  uint8_t bit = (1 << (y & 7));
  *pixel &= ~bit;
  *pixel |= color ? bit : 0;
}

void SSD1306::area(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, RGB color)
{
  for (uint8_t xx = x; xx <= x1; xx++)
    for (uint8_t yy = x; yy <= y1; yy++)
      pixel(xx, yy, color);
}

void SSD1306::update(void)
{
  // send_command(SetPageAddress, 0, 0xFF);
  // send_command(SetColumnAddress, 0, max_x());

  // i2c.begin();
  // i2c.write((uint8_t)0x40);

  i2c.begin();
  i2c.write(0);
  i2c.write(SetColumnAddress);
  i2c.write(0);
  i2c.write(max_x());
  i2c.write(SetPageAddress);
  i2c.write(0);
  i2c.write(0xFF);
  i2c.write(SetDisplayStartLine);
  i2c.write(buffer, sizeof(buffer));
  i2c.end();
}

#endif
