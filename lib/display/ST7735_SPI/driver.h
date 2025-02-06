#pragma once
#include "spi/SPI.h"
#include "pins.h"
#include "const/ST7735.h"
#include "type/include.h"

#define LCD_DRIVER    ST7735_SPI

template<typename C>
class ST7735_SPI {
public:

  inline const uint16_t max_x() { return MAX_X; }
  inline const uint16_t max_y() { return MAX_Y; }
  void init()
  {
    L_RST(OUT);L_CS(OUT);L_RS(OUT);
    L_CS(SET);L_RS(SET);
    SPI.init();

    L_RST(CLR);               // Аппаратный сброс
    delay_us(2000);
    L_RST(SET);
    delay_us(15000);          // Ждать стабилизации напряжений
    L_CS(CLR);                // CS Выбор дисплея

    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(LCD_FLIP);
    set_rgb_format();
    send_command(DISPON); // Display On
    SPI.wait();

    L_CS(SET);
  }

protected:
  // inline void select() { L_CS(CLR); }
  // inline void release() { L_CS(SET); }

  void send_byte(uint8_t data) { SPI.send(data); }
  void send_command(uint8_t command)
  {
  SPI.wait();
  L_RS(CLR); // Запись команды
  SPI.send(command);
  SPI.wait();
  L_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); // Column Address Set
    SPI.send16(x0);
    SPI.send16(x1);

    send_command(RASET); // Row Address Set
    SPI.send16(y0);
    SPI.send16(y1);

    send_command(RAMWR); // Memory Write
  }

  void send_rgb(C color)
  {
    SPI.send(color.red);
    SPI.send(color.green);
    SPI.send(color.blue);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    L_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      SPI.send(color.red);
      SPI.send(color.green);
      SPI.send(color.blue);
    }
    SPI.wait();
    L_CS(SET);
  }
  
private:
   SPI_Master SPI;
  void set_rgb_format();
  virtual  void send_config(const uint8_t *, uint8_t) = 0;
};

template<>
  inline void ST7735_SPI<RGB16>::send_rgb(RGB16 color) 
  { 
    SPI.send16(color.rgb); 
  }

template<>
  inline void ST7735_SPI<RGB12>::send_rgb(RGB12 color) 
  {
    static uint8_t half, flag = 0;

  if (flag) {
    SPI.send(half | (color.rgb >> 8));
    flag = 0;
    SPI.send(color.rgb);
  }
  else {
    SPI.send(color.rgb >> 4);
    half = color.rgb << 4;
    flag = 1;
  }
  }

template<>
  void ST7735_SPI<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    L_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
    
    while (len--) SPI.send16(color.rgb);
    SPI.wait();
    L_CS(SET);
  }

template<>
  void ST7735_SPI<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    L_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1))>>1;

    uint8_t hbyte = color.rgb >> 4;
    uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
    uint8_t lbyte = color.rgb;

    while (len--) {
      SPI.send(hbyte);
      SPI.send(mbyte);
      SPI.send(lbyte);
    }
    SPI.wait();
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
 