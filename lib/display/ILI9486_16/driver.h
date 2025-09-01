#pragma once
#include "comon/include.h"
#include "pins.h"

template <typename C>
class ILI9486_16 : public IDriver {
private:
  uint16_t flag;
  ATTR_INLINE void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 319; }
  ATTR_INLINE constexpr int16_t max_y() { return 479; }
  ATTR_INLINE void select() { L_CS(CLR); }
  ATTR_INLINE void release() { L_CS(SET); }
  ATTR_INLINE void send_rgb(C color, int32_t len) {
    while (len--) send_rgb(color);
  }

  void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0) {
    GPIO_ILI9486_16();

    select();
    send_command(SLPOUT);
    delay_ms(30);
    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL);
    send_byte((rotation | 0x08));  // BGR -> RBG
    set_rgb_format();
    send_command(NORON);
    send_command(DISPON);
    release();
  }

  ATTR_INLINE void send_command(uint8_t command) {
    L_RS(CLR);
    send_byte(command);
    L_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data) {
    L_PORT(OUTPUT) = data;
    L_WR(CLR);
    L_WR(SET);
  }

  ATTR_INLINE void send_word(uint16_t data) {
    L_PORT(OUTPUT) = (data >> 8);
    L_WR(CLR);
    L_WR(SET);
    L_PORT(OUTPUT) = data;
    L_WR(CLR);
    L_WR(SET);
  }

  void pixel(int16_t x, int16_t y, C color) {
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }
};

////////////////////////////////// RGB16 //////////////////////////////////////

template <>
void ILI9486_16<RGB16>::set_rgb_format() {
  send_command(COLMOD);
  send_byte(0x55);  // 5x6x5 bit
}

template <>
ATTR_INLINE void ILI9486_16<RGB16>::send_rgb(RGB16 color) {
  L_PORT(OUTPUT) = color.rgb;
  L_WR(CLR);
  L_WR(SET);
}

template <>
ATTR_INLINE void ILI9486_16<RGB16>::send_rgb(RGB16 color, int32_t len) {
  L_PORT(OUTPUT) = color.rgb;
  while (len--) {
    L_WR(CLR);
    L_WR(SET);
  }
}

template <>
void ILI9486_16<RGB16>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color) {
  select();
  set_addr(x0, y0, x1, y1);

  uint32_t len = (uint32_t)(x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1);
  L_PORT(OUTPUT) = color.rgb;

#ifndef WR_FORSED
  while (len--) {
  #ifdef SYSCLK_FREQ_144MHz_HSE
    L_WR(CLR);
  #endif
    L_WR(CLR);
    L_WR(SET);
  }
#else

  #ifdef CH32V20x_D6
  if (len > 26) {  // Порог эффективности
    L_WR(OUTA);

    len--;
    for (int i = len >> 16; i > 0; i--) {
      TIM3->CNT = 0xFFFF;
      TIM3->CTLR1 = TIM_DIR | TIM_OPM | TIM_CEN;  // Включеие счётчика
      TIM3->INTFR = 0;
      while ((TIM3->INTFR & TIM_UIF) == 0);
    }

    if (TIM3->CNT = len) {
      TIM3->INTFR = 0;
      TIM3->CTLR1 = TIM_DIR | TIM_OPM | TIM_CEN;  // Включеие счётчика
      while ((TIM3->INTFR & TIM_UIF) == 0);
    }

    L_WR(OUT);
  } else if (F_CPU > 120000000)
    while (len--) {
      L_WR(CLR);
      L_WR(CLR);
      L_WR(SET);
    }
  else
    while (len--) {
      L_WR(CLR);
      L_WR(SET);
    }
  #endif

  #ifdef MIK32V2
  while (len--) {
    L_WR(CLR);
    L_WR(SET);
  }
  #endif

#endif

  release();
}

////////////////////////////////// RGB18 //////////////////////////////////////

template <>
ATTR_INLINE void ILI9486_16<RGB18>::set_rgb_format() {
  send_command(COLMOD);
  send_byte(0x66);  // 6x6x6 bit (24 bit transfer)
}

template <>
ATTR_INLINE void ILI9486_16<RGB18>::select() {
  L_CS(CLR);
  flag = 0;
}

template <>
void ILI9486_16<RGB18>::send_rgb(RGB18 color) {
  static uint16_t half;
  if (flag) {
    L_PORT(OUTPUT) = color.red | half;
    L_WR(SET);
    L_WR(CLR);
    flag = 0;
    L_PORT(OUTPUT) = color.rgb;
    L_WR(SET);
    L_WR(CLR);
  } else {
    L_PORT(OUTPUT) = color.rgb >> 8;
    L_WR(SET);
    L_WR(CLR);
    half = color.blue << 8;
    flag = 1;
  }
}

template <>
void ILI9486_16<RGB18>::area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color) {
  select();
  set_addr(x0, y0, x1, y1);
  uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1)) >> 1;
  uint16_t hword = *(uint16_t *)&color.green;
  uint16_t mword = color.red | (color.blue << 8);
  uint16_t lword = *(uint16_t *)&color.blue;

  while (len--) {
    L_PORT(OUTPUT) = hword;
    L_WR(SET);
    L_WR(CLR);

    L_PORT(OUTPUT) = mword;
    L_WR(SET);
    L_WR(CLR);

    L_PORT(OUTPUT) = lword;
    L_WR(SET);
    L_WR(CLR);
  }
  release();
}