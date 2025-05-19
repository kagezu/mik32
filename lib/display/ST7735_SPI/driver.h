#pragma once
#include "SPI.h"
#include "pins.h"
#include "const.h"
#include "type/include.h"

#define LCD_DRIVER    ST7735_SPI
#define SPI_THR_2     7
#define SPI_THR_3     6

extern SPI spi;

template<typename C>
class ST7735_SPI {
public:
inline constexpr uint16_t max_x() { return LCD_FLIP & EX_X_Y ? MAX_Y : MAX_X;}
inline constexpr uint16_t max_y() { return LCD_FLIP & EX_X_Y ? MAX_X : MAX_Y;}
  void init()
  {
    L_RST(OUT);L_RST(CLR);
    L_CS(OUT);L_CS(SET);
    L_RS(OUT);L_RS(SET);
    L_RST(SET);

    _spi.init();
    _spi.thr(SPI_THR_2);
    
    delay_ms(10);
    select();             // CS Выбор дисплея
    send_command(SWRESET);	// Out of sleep mode
    delay_ms(50);
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(LCD_FLIP);
    set_rgb_format();
    send_command(DISPON); // Display On
    release();
  }

protected:
  inline void select() { spi.begin(_spi); L_CS(CLR); }
  inline void release() { spi.end(); L_CS(SET); }

  void send_byte(uint8_t data) { spi.send(data); spi.wait_clr(); }
  void send_command(uint8_t command)
  {
    L_RS(CLR); // Запись команды
    send_byte(command);
    L_RS(SET); // Запись данных
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); // Column Address Set
    spi.send16(x0);
    spi.wait();
    spi.send16(x1);
    spi.wait_clr();

    send_command(RASET); // Row Address Set
    spi.send16(y0);
    spi.wait();
    spi.send16(y1);
    spi.wait_clr();

    send_command(RAMWR); // Memory Write
  }

  void send_rgb(C color)
  {
    spi.wait_thr();
    spi.send(color.red);
    spi.wait();
    spi.send(color.green);
    spi.wait();
    spi.send(color.blue);
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
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
  
private:
  SPI_Settings _spi;
  void set_rgb_format();
  void send_config(const uint8_t *config, uint8_t size)
  {
    while (size) {
      uint8_t count = pgm_read_byte(config++);
      uint8_t comand = pgm_read_byte(config++);
      size -= 2 + count;
      send_command(comand);
      while (count--) send_byte(pgm_read_byte(config++));
    }
  }
};

template<>
  inline void ST7735_SPI<RGB16>::send_rgb(RGB16 color) 
  { 
    spi.wait_thr();
    spi.send16(color.rgb); 
  }

template<>
  inline void ST7735_SPI<RGB12>::send_rgb(RGB12 color) 
  {
    static uint8_t half, flag = 0;

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
  void ST7735_SPI<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
    
    while (len--) {spi.wait_thr();  spi.send16(color.rgb); }
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
      spi.wait_thr();
      spi.send(hbyte);
      spi.wait();
      spi.send(mbyte);
      spi.wait();
      spi.send(lbyte);
    }
    release();
  }

template<>
 void ST7735_SPI<RGB32>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  _spi.thr(SPI_THR_3);
 }
 template<>
 void ST7735_SPI<RGB18>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x06); // 6x6x6 bit (24 bit transfer)
  _spi.thr(SPI_THR_3);
 }
template<>
 void ST7735_SPI<RGB16>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x05); // 5x6x5 bit
  _spi.thr(SPI_THR_2);
 }
template<>
 void ST7735_SPI<RGB12>::set_rgb_format(){
  send_command(COLMOD);
  send_byte(0x03); // 4x4x4 bit
  _spi.thr(SPI_THR_3);
 }
 