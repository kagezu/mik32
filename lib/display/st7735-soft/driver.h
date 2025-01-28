#pragma once
#include "type/include.h"
#include "pin.h"
#include "const.h"

template<typename C>
class ST7735_SOFT : IDriver<C> {
public:
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color){}
  void init()
  {
    /*
  L_SCK(GPIO); L_SDA(GPIO); L_RST(GPIO); L_CS(GPIO); L_RS(GPIO);
    L_SCK(OUT); L_SDA(OUT); L_RST(OUT); L_CS(OUT); L_RS(OUT);

  SELA45(GPIO);
  SELA45(OUT);
  SELA45(RES);

    L_RST(RES);           // Аппаратный сброс
    delay_us(2000);

    L_SCK(RES);
    L_SDA(RES);
    L_RST(SET);
    delay_us(2000);         // Ждать стабилизации напряжений
    L_CS(RES);            // CS Выбор дисплея
    send_command(SLPOUT); // Проснуться
    delay_us(2000);         // Ждать стабилизации напряжений
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(LCD_FLIP);
    set_rgb_format();
    send_command(DISPON); // Display On
    L_CS(SET);
*/


  L_SCK(GPIO);
  L_SDA(GPIO);
  L_RST(GPIO);
  L_CS(GPIO);
  L_RS(GPIO);

  L_SCK(OUT);
  L_SDA(OUT);
  L_RST(OUT);
  L_CS(OUT);
  L_RS(OUT);


  SELA45(GPIO);
  SELA45(OUT);
  SELA45(RES);


  L_SCK(RES);
  L_SDA(RES);


  L_RST(SET);
  delay_us(1000); // Ждать стабилизации напряжений
  L_CS(RES);          // CS Выбор дисплея
  send_command(SLPOUT);      // Проснуться
  delay_us(1000); // Ждать стабилизации напряжений

  send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));

  send_command(MADCTL);
  send_byte(
  #ifdef EX_X_Y
    0x20 |
  #endif
  #ifdef FLIP_X
    0x40 |
  #endif
  #ifdef FLIP_Y
    0x80 |
  #endif
    0x00);

  send_command(COLMOD);
  send_byte(_RGB18);
  send_command(DISPON); // Display On

  L_CS(SET);



  }

private:
  using IDriver<C>::send_config;
  void set_rgb_format();

protected:
  void send_rgb(C color){}


  void send_command(uint8_t command)
  {
    L_RS(RES); // Запись команды
    send_byte(command);
    L_RS(SET); // Запись данных
  };

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(CASET); // Column Address Set
    send_zero();
    send_byte(x0);
    send_zero();
    send_byte(x1);

    send_command(RASET); // Row Address Set
    send_zero();
    send_byte(y0);
    send_zero();
    send_byte(y1);

    send_command(RAMWR); // Memory Write
  };

  void send_zero()
  {
    // reg count = 8;
    // L_SDA(RES);
    // while(count--){
    //   L_SCK(SET);
    //   L_SCK(RES);
    // }

  L_SDA(RES);

  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);
  L_SCK(SET);
  L_SCK(RES);



  }

  void send_byte(uint8_t data)
  {


    // for( reg mask = 0x80; mask > 0; mask >>=1){
    //   if (data & mask) L_SDA(SET);
    //   else L_SDA(RES);
    //   L_SCK(SET);
    //   L_SCK(RES);
    // }


  reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
  reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);


  if (data & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x2) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  if (data & 0x1) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
  else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

  L_SCK(RES);







  }
};

template<>
  void ST7735_SOFT<RGB32>::send_rgb(RGB32 color)
  {
    uint8_t r = color.red;
    uint8_t g = color.green;
    uint8_t b = color.blue;

    reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
    reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
    reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
    reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

    if (r & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

    if (g & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

    if (b & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

    L_SCK(RES);
  }

template<>
  void ST7735_SOFT<RGB16>::send_rgb(RGB16 color)
  {
    uint16_t rgb = color.rgb;

    reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
    reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
    reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
    reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

    if (rgb & 0x8000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x4000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x2000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x1000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x800) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x400) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x200) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x100) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x2) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x1) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  
    L_SCK(RES);
  }

template<>
  void ST7735_SOFT<RGB12>::send_rgb(RGB12 color)
  {
    uint16_t rgb = color.rgb;

    reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
    reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
    reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
    reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

    if (rgb & 0x800) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x400) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x200) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x100) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x2) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x1) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
  
    L_SCK(RES);
  }

template<>
  void ST7735_SOFT<RGB32>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB32 color)
  {
    uint8_t r = color.red;
    uint8_t g = color.green;
    uint8_t b = color.blue;

    L_CS(RES);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
    reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
    reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
    reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

    // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
    while (len--) {
    if (r & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (r & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

    if (g & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (g & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;

    if (b & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (b & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    L_SCK(MMO) = d0;
    L_SCK(MMO) = s0;
    }
    L_SCK(RES);
    L_CS(SET);
  }

template<>
  void ST7735_SOFT<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    uint16_t rgb = color.rgb;

    L_CS(RES);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
    reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
    reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
    reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

    // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
    while (len--) {
    if (rgb & 0x8000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x4000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x2000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x1000) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x800) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x400) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x200) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x100) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x2) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x1) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    }
    L_SCK(RES);
    L_CS(SET);
  }

template<>
  void ST7735_SOFT<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    uint16_t rgb = color.rgb;

    L_CS(RES);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    reg d0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
    reg d1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
    reg s0 = (L_SCK(MMO) & ~L_SDA(MASK)) | L_SCK(MASK);
    reg s1 = L_SCK(MMO) | L_SDA(MASK) | L_SCK(MASK);

    // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
    while (len--) {
    if (rgb & 0x800) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x400) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x200) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x100) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x80) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x40) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x20) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x10) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }

    if (rgb & 0x8) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x4) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x2) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    if (rgb & 0x1) { L_SCK(MMO) = d1; L_SCK(MMO) = s1; }
    else { L_SCK(MMO) = d0; L_SCK(MMO) = s0; }
    }
    L_SCK(RES);
    L_CS(SET);
  }

template<>
 void ST7735_SOFT<RGB32>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB18);
 }
template<>
 void ST7735_SOFT<RGB16>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB16);
 }
template<>
 void ST7735_SOFT<RGB12>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(_RGB12);
 }
 