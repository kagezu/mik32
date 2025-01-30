#pragma once
#include "pin.h"
#include "const.h"
#include "type/include.h"

#define SPI_WAIT  while (!(SPSR & _BV(SPIF)));

template<typename C>
class ST7735_SPI : IDriver<C> {
public:
  void init()
  {

#ifdef __AVR_ATmega328P__

  L_SCK(OUT);L_SDA(OUT);SS(OUT);
  L_RST(OUT);L_CS(OUT);L_RS(OUT);

    SPCR = _BV(SPE) | _BV(MSTR);
    SPSR = _BV(SPI2X);
    TCCR0B |= _BV(CS00);
    SPDR = 0;

#endif


#ifdef MIK32V2
  L_SCK(GPIO); L_SDA(GPIO);L_RST(GPIO);L_CS(GPIO);L_RS(GPIO);
#endif


  L_RST(RES);               // Аппаратный сброс
  delay_us(2000);
  L_RST(SET);
  delay_us(15000);          // Ждать стабилизации напряжений
  L_CS(RES);                // CS Выбор дисплея

  send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
  send_command(MADCTL);
  send_byte(LCD_FLIP);
  set_rgb_format();
  send_command(DISPON); // Display On

  L_CS(SET);
  }

private:
  using IDriver<C>::send_config;
  void set_rgb_format();

protected:
  void send_command(uint8_t command)
  {
  SPI_WAIT;
  L_RS(RES); // Запись команды
  SPDR = command;
  asm volatile("nop");
  SPI_WAIT;
  L_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); // Column Address Set
    send_byte(0);
    send_byte(x0);
    send_byte(0);
    send_byte(x1);

    send_command(RASET); // Row Address Set
    send_byte(0);
    send_byte(y0);
    send_byte(0);
    send_byte(y1);

    send_command(RAMWR); // Memory Write
  }

  void send_byte(uint8_t data)
  {
    SPI_WAIT;
    SPDR = data;
  }

  void send_rgb(C color)
  {
    SPI_WAIT;
    SPDR = color.blue;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.green;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.red;
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    L_CS(RES);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
    SPI_WAIT;
    SPDR = color.blue;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.green;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.red;
    }
    L_CS(SET);
  }
};

template<>
  void ST7735_SPI<RGB16>::send_rgb(RGB16 color)
  {
    uint16_t rgb = color.rgb;
    SPI_WAIT;
    SPDR = rgb >> 8;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = rgb;
  }

template<>
  void ST7735_SPI<RGB12>::send_rgb(RGB12 color)
  {
    uint16_t rgb = color.rgb;

  static uint8_t half, flag = 0;
  uint8_t data;

  if (flag) {
    data = half | (rgb >> 8);
    SPI_WAIT;
    SPDR = data;
    flag = 0;
    SPI_WAIT;
    SPDR = (uint8_t)rgb;
  }
  else {
    data = rgb >> 4;
    SPI_WAIT;
    SPDR = data;
    half = rgb << 4;
    flag = 1;
  }
  
  }

template<>
  void ST7735_SPI<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    uint16_t rgb = color.rgb;

    L_CS(RES);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
    SPI_WAIT;
    SPDR = rgb >> 8;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = (uint8_t)rgb;
    }
    L_CS(SET);
  }

template<>
  void ST7735_SPI<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    uint16_t rgb = color.rgb;

    L_CS(RES);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    uint8_t hbyte = color.rgb >> 4;
    uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
    uint8_t lbyte = color.rgb;
    len >>= 1;

    while (len--) {
    SPI_WAIT;
    SPDR = hbyte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = mbyte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = lbyte;
    }
    L_CS(SET);
  }

template<>
 void ST7735_SPI<RGB32>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB18);
 }
 template<>
 void ST7735_SPI<RGB18>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB18);
 }
template<>
 void ST7735_SPI<RGB16>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB16);
 }
template<>
 void ST7735_SPI<RGB12>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB12);
 }
 