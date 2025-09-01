#pragma once
#include "comon/include.h"
#include "const.h"
#include "pins.h"
#ifdef MIK32V2
  #include "timer.h"
#endif

template <typename C>
class NT35510 {};


////////////////////////////////// RGB16 //////////////////////////////////////

template <>
class NT35510<RGB16> {
public:
  using RGB = RGB16;

#include "base.h"

  ATTR_INLINE void select() { NT_CS(CLR); }
  ATTR_INLINE void release() { NT_CS(SET); }

  void set_rgb_format() {
    send_command(NT_COLMOD);
    send_byte(0x05);  // 5x6x5 bit
  }

  ATTR_INLINE void send_rgb(RGB16 color) {
    NT_PORT(OUTPUT) = color.rgb;
    NT_WR(CLR);
    NT_WR(SET);
  }

  ATTR_INLINE void send_rgb(RGB16 color, int32_t len) {
    NT_PORT(OUTPUT) = color.rgb;
    while (len--) {
      NT_WR(CLR);
      NT_WR(SET);
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color) {
    select();
    set_addr(x0, y0, x1, y1);

    uint32_t len = (x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1);
    NT_PORT(OUTPUT) = color.rgb;

#ifndef WR_FORSED
    while (len--) {
      NT_WR(CLR);
      NT_WR(SET);
    }
#else

  #ifdef CH32V20x_D6
    if (len > 26) { // Порог эффективности
      L_WR(OUTA);

      len -= 2;
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
    } else
      while (len--) {
        NT_WR(CLR);
        NT_WR(SET);
      }
  #endif

  #ifdef MIK32V2
    len <<= 1;
    T32_1_TOP(-1);
    NT_WR(OUTA);
    TIMER32_2->CHANNELS[0].CNTRL =
      TIMER32_CH_CNTRL_MODE_PWM_M |
      TIMER32_CH_CNTRL_ENABLE_M;
    T32_1_C;
    while (T32_1 < len);  // NT_PORT(OUTPUT) = T32_1 * color.rgb;
    TIMER32_2->CHANNELS[0].CNTRL = 0;
    NT_WR(GPIO);
  #endif
#endif

    release();
  }
};

////////////////////////////////// RGB18 //////////////////////////////////////

template <>
class NT35510<RGB18> {
private:
  bool flag;

public:
  using RGB = RGB18;

#include "base.h"

  ATTR_INLINE void select() {
    NT_CS(CLR);
    flag = 0;
  }
  ATTR_INLINE void release() {
    NT_CS(CLR);
  }
  ATTR_INLINE void send_rgb(RGB color, int32_t len) {
    while (len--) send_rgb(color);
  }
  ATTR_INLINE void set_rgb_format() {
    send_command(NT_COLMOD);
    send_byte(0x06);  // 6x6x6 bit
  }

  void send_rgb(RGB18 color) {
    static uint16_t half;
    if (flag) {
      NT_PORT(OUTPUT) = color.red | half;
      NT_WR(CLR);
      NT_WR(SET);
      flag = 0;
      NT_PORT(OUTPUT) = color.rgb;
      NT_WR(CLR);
      NT_WR(SET);
    } else {
      NT_PORT(OUTPUT) = (color.rgb >> 8);
      NT_WR(CLR);
      NT_WR(SET);
      half = color.blue << 8;
      flag = 1;
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color) {
    select();
    set_addr(x0, y0, x1, y1);
    uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1) + 1) >> 1;
    uint16_t hword = *(uint16_t *)&color.green;
    uint16_t mword = color.red | (color.blue << 8);
    uint16_t lword = *(uint16_t *)&color.blue;

    while (len--) {
      NT_PORT(OUTPUT) = hword;
      NT_WR(CLR);
      NT_WR(SET);

      NT_PORT(OUTPUT) = mword;
      NT_WR(CLR);
      NT_WR(SET);

      NT_PORT(OUTPUT) = lword;
      NT_WR(CLR);
      NT_WR(SET);
    }
    release();
  }
};

////////////////////////////////// RGB24 //////////////////////////////////////

template <>
class NT35510<RGB24> {
private:
  bool flag;

public:
  using RGB = RGB24;

#include "base.h"

  ATTR_INLINE void select() {
    NT_CS(CLR);
    flag = 0;
  }
  ATTR_INLINE void release() {
    NT_CS(CLR);
  }
  ATTR_INLINE void send_rgb(RGB color, int32_t len) {
    while (len--) send_rgb(color);
  }

  ATTR_INLINE void set_rgb_format() {
    send_command(NT_COLMOD);
    send_byte(0x07);  // 8x8x8 bit
  }

  void send_rgb(RGB24 color) {
    static uint16_t half;
    if (flag) {
      NT_PORT(OUTPUT) = color.red | half;
      NT_WR(CLR);
      NT_WR(SET);
      flag = 0;
      NT_PORT(OUTPUT) = color.rgb;
      NT_WR(CLR);
      NT_WR(SET);
    } else {
      NT_PORT(OUTPUT) = (color.rgb >> 8);
      NT_WR(CLR);
      NT_WR(SET);
      half = color.blue << 8;
      flag = 1;
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB24 color) {
    select();
    set_addr(x0, y0, x1, y1);
    uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1) + 1) >> 1;
    uint16_t hword = *(uint16_t *)&color.green;
    uint16_t mword = color.red | (color.blue << 8);
    uint16_t lword = *(uint16_t *)&color.blue;

    while (len--) {
      NT_PORT(OUTPUT) = hword;
      NT_WR(CLR);
      NT_WR(SET);

      NT_PORT(OUTPUT) = mword;
      NT_WR(CLR);
      NT_WR(SET);

      NT_PORT(OUTPUT) = lword;
      NT_WR(CLR);
      NT_WR(SET);
    }
    release();
  }
};
