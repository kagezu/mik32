#pragma once
#include "pins.h"
#include "type/include.h"

template<typename C = RGB16>
class ST7735_SOFT {
public:
  // Разрешение дисплея
  GCC_INLINE constexpr int16_t max_x() { return 127; }
  GCC_INLINE constexpr int16_t max_y() { return 159; }
  
  void init(uint8_t position)
  {
  ST_SOFT_SCK(GPIO); ST_SOFT_SDA(GPIO);ST_SOFT_RST(GPIO);ST_SOFT_CS(GPIO);ST_SOFT_RS(GPIO);
  ST_SOFT_SCK(OUT);ST_SOFT_SDA(OUT);ST_SOFT_RST(OUT);ST_SOFT_CS(OUT);ST_SOFT_RS(OUT);
  ST_SOFT_CS(SET);ST_SOFT_RS(SET);

  ST_SOFT_RST(CLR);               // Аппаратный сброс
  delay_us(2000);
  ST_SOFT_RST(SET);
  delay_us(15000);          // Ждать стабилизации напряжений
  ST_SOFT_CS(CLR);                // CS Выбор дисплея

  send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
  send_command(MADCTL);
  send_byte(position);
  set_rgb_format();
  send_command(DISPON); // Display On

  ST_SOFT_CS(SET);
  }

protected:
  inline void select() { ST_SOFT_CS(CLR); }
  inline void release() { ST_SOFT_SCK(CLR); ST_SOFT_CS(SET); }

  void send_command(uint8_t command)
  {
    ST_SOFT_RS(CLR); // Запись команды
    send_byte(command);
    ST_SOFT_RS(SET); // Запись данных
  }

  GCC_NO_INLINE void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
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
  }

  GCC_NO_INLINE void send_zero()
  {
    reg count = 8;
    ST_SOFT_SDA(CLR);
    while(count--){
      ST_SOFT_SCK(SET);
      ST_SOFT_SCK(CLR);
    }
  }

  GCC_NO_INLINE void send_byte(uint8_t data)
  {
    for( reg mask = 0x80; mask > 0; mask >>=1){
      if (data & mask) ST_SOFT_SDA(SET);
      else ST_SOFT_SDA(CLR);
      ST_SOFT_SCK(SET);
      ST_SOFT_SCK(CLR);
    }
  }

  void send_rgb(uint8_t red, uint8_t green, uint8_t blue){
    send_rgb(C(red, green, blue));
  }

  GCC_NO_INLINE void send_rgb(C color)
  {
    uint8_t r = color.red;
    uint8_t g = color.green;
    uint8_t b = color.blue;

    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
    reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);

    if (r & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    if (g & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    if (b & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    ST_SOFT_SCK(CLR);
  }

   GCC_NO_INLINE void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color)
  {
    uint8_t r = color.red;
    uint8_t g = color.green;
    uint8_t b = color.blue;

    ST_SOFT_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
    reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);

    // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
    while (len--) {
    if (r & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (r & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    if (g & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (g & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;

    if (b & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (b & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    }
    ST_SOFT_SCK(CLR);
    ST_SOFT_CS(SET);
  }

private:
GCC_NO_INLINE void set_rgb_format();
  virtual void send_config(const uint8_t *config, uint8_t size) = 0;
};

template<>
GCC_NO_INLINE void ST7735_SOFT<RGB16>::send_rgb(RGB16 color)
  {
    uint16_t rgb = color.rgb;

    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
    #ifndef __AVR__
    reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);
    #endif

    #ifdef __AVR__
    ST_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    
    ST_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    
    ST_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    
    ST_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    ST_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
    ST_SOFT_SCK(OUTPUT) = s0;
    #else
  reg mask = 0x8000;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
  if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
  #endif
  
    // ST_SOFT_SCK(CLR);
  }

  template<>
  GCC_NO_INLINE void ST7735_SOFT<RGB16>::send_rgb(uint8_t red, uint8_t green, uint8_t blue)
    {  
      reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
      reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
      reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
      #ifndef __AVR__
      reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);
      #endif
  
      #ifdef __AVR__

      ST_SOFT_SCK(OUTPUT) = red & 0x80 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = red & 0x40 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = red & 0x20 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = red & 0x10 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = red & 0x8 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;

      ST_SOFT_SCK(OUTPUT) = green & 0x80 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = green & 0x40 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = green & 0x20 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = green & 0x10 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = green & 0x8 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = green & 0x4 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;

      ST_SOFT_SCK(OUTPUT) = blue & 0x80 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = blue & 0x40 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = blue & 0x20 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = blue & 0x10 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = blue & 0x8 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;

      #else
      if (red & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (red & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (red & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (red & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (red & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }

      if (green & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (green & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (green & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (green & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (green & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (green & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }

      if (blue & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (blue & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (blue & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (blue & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
      if (blue & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
      else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    #endif
    
      ST_SOFT_SCK(CLR);
    }

template<>
GCC_NO_INLINE void ST7735_SOFT<RGB12>::send_rgb(RGB12 color)
  {
    uint16_t rgb = color.rgb;

    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
    reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);

    if (rgb & 0x800) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x400) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x200) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x100) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }

    if (rgb & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }

    if (rgb & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x2) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x1) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
  
    ST_SOFT_SCK(CLR);
  }

template<>
GCC_NO_INLINE void ST7735_SOFT<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    uint16_t rgb = color.rgb;

    ST_SOFT_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
    #ifndef __AVR__
    reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);
    #endif

    // Дублирование кода намеренно, так как оптимизация ускоряет тут выполнение в 2 раза
    while (len--) {
      #ifdef __AVR__
      ST_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      
      ST_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      
      ST_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      
      ST_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      #else
      reg mask = 0x8000;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; } mask >>=1;
      if (rgb & mask) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; } else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    #endif
    }
    ST_SOFT_SCK(CLR);
    ST_SOFT_CS(SET);
  }

template<>
GCC_NO_INLINE void ST7735_SOFT<RGB12>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB12 color)
  {
    uint16_t rgb = color.rgb;

    ST_SOFT_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

    reg d0 = ST_SOFT_SCK(OUTPUT) & ~(ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK));
    reg d1 = (ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK)) & ~ST_SOFT_SCK(MASK);
    reg s0 = (ST_SOFT_SCK(OUTPUT) & ~ST_SOFT_SDA(MASK)) | ST_SOFT_SCK(MASK);
    #ifndef __AVR__
    reg s1 = ST_SOFT_SCK(OUTPUT) | ST_SOFT_SDA(MASK) | ST_SOFT_SCK(MASK);
    #endif

    // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
    while (len--) {
      #ifdef __AVR__      
      ST_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      
      ST_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      
      ST_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      ST_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
      ST_SOFT_SCK(OUTPUT) = s0;
      #else
    if (rgb & 0x800) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x400) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x200) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x100) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }

    if (rgb & 0x80) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x40) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x20) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x10) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }

    if (rgb & 0x8) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x4) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x2) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    if (rgb & 0x1) { ST_SOFT_SCK(OUTPUT) = d1; ST_SOFT_SCK(OUTPUT) = s1; }
    else { ST_SOFT_SCK(OUTPUT) = d0; ST_SOFT_SCK(OUTPUT) = s0; }
    #endif
    }
    ST_SOFT_SCK(CLR);
    ST_SOFT_CS(SET);
  }

template<>
 void ST7735_SOFT<RGB32>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(0x06); // 6x6x6 bit (24 bit transfer)
 }
template<>
 void ST7735_SOFT<RGB16>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(0x05); // 5x6x5 bit
 }
template<>
 void ST7735_SOFT<RGB12>::set_rgb_format(){
    send_command(COLMOD);
    send_byte(0x03); // 4x4x4 bit
 }
 