#pragma once
#include "spi/SPI.h"
#include "pins.h"
#include "const.h"
#include "type/include.h"

#define LCD_DRIVER    ST7735_SPI
#ifdef __AVR__
extern SPI_Class SPI;
#endif

template<typename C>
class ST7735_SPI {
public:
inline constexpr uint16_t max_x() { return LCD_FLIP & EX_X_Y ? MAX_Y : MAX_X;}
inline constexpr uint16_t max_y() { return LCD_FLIP & EX_X_Y ? MAX_X : MAX_Y;}
  void init()
  {
    L_RST(OUT);L_CS(OUT);L_RS(OUT);
    L_CS(SET);L_RS(SET);L_RST(CLR);
    _spi.init();
    L_RST(SET);
    delay_us(15000);          // Ждать стабилизации напряжений
    select();                  // CS Выбор дисплея
    send_command(SWRESET);
    delay_us(15000);          // Ждать стабилизации напряжений

    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(LCD_FLIP);
    set_rgb_format();
    send_command(DISPON); // Display On

    release();
  }

protected:
  inline void select() { SPI.begin(_spi); L_CS(CLR); }
  inline void release() { SPI.wait(); L_CS(SET); SPI.end(); }

  void send_byte(uint8_t data) { SPI.send(data);SPI.wait(); }
  void send_command(uint8_t command)
  {
  L_RS(CLR); // Запись команды
  send_byte(command);
  L_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); // Column Address Set
    SPI.send16(x0);
    SPI.wait();
    SPI.send16(x1);
    SPI.wait();

    send_command(RASET); // Row Address Set
    SPI.send16(y0);
    SPI.wait();
    SPI.send16(y1);
    SPI.wait();

    send_command(RAMWR); // Memory Write
  }

  void send_rgb(C color)
  {
    SPI.wait();
    SPI.send(color.red);
    SPI.wait();
    SPI.send(color.green);
    SPI.wait();
    SPI.send(color.blue);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    while (len--) {
      SPI.send(color.red);
      SPI.wait();
      SPI.send(color.green);
      SPI.wait();
      SPI.send(color.blue);
      SPI.wait();
    }
    release();
  }
  
private:
  SPI_Settings _spi;
  void set_rgb_format();
  virtual  void send_config(const uint8_t *, uint8_t) = 0;
};

template<>
  inline void ST7735_SPI<RGB16>::send_rgb(RGB16 color) 
  { 
    SPI.wait();
    SPI.send16(color.rgb); 
  }

template<>
  inline void ST7735_SPI<RGB12>::send_rgb(RGB12 color) 
  {
    static uint8_t half, flag = 0;

  if (flag) {
    SPI.wait();
    SPI.send(half | (color.rgb >> 8));
    flag = 0;
    SPI.wait();
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
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
    
    while (len--) { SPI.send16(color.rgb); SPI.wait(); }
    // SPI.wait();
    release();
  }

template<>
  void ST7735_SPI<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = ((x1 - x0 + 1) * (y1 - y0 + 1))>>1;

    uint8_t hbyte = color.rgb >> 4;
    uint8_t mbyte = (color.rgb << 4) | ((color.rgb & 0xf00) >> 8);
    uint8_t lbyte = color.rgb;

    while (len--) {
      SPI.send(hbyte);
      SPI.wait();
      SPI.send(mbyte);
      SPI.wait();
      SPI.send(lbyte);
      SPI.wait();
    }
    release();
  }

template<>
 void ST7735_SPI<RGB32>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
 }
 template<>
 void ST7735_SPI<RGB18>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
 }
template<>
 void ST7735_SPI<RGB16>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
 }
template<>
 void ST7735_SPI<RGB12>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
 }
 