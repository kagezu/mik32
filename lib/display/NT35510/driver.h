#pragma once
#include "comon/include.h"
#include "const.h"
#include "pins.h"
#ifdef MIK32V2
#include "timer.h"
#endif
#ifdef CH32V20x_D6
#include "timer16.h"
#endif

template <typename C>
class NT35510 {};


////////////////////////////////// RGB16 //////////////////////////////////////

template <>
class NT35510<RGB16> {
protected:
#ifdef MIK32V2
  // T32<1> tim1;
  // T32<2> tim2;
#endif

public:
  using RGB = RGB16;

#include "base.h"

  INLINE void select() { NT_CS.clr(); }
  INLINE void release() { NT_CS.set(); }

  void set_rgb_format()
  {
    send_command(NT_COLMOD);
    send_byte(0x05);  // 5x6x5 bit

    /*
    tim1.start();
    tim2.top(1);
    tim2.ocr(1);
    tim2.start();
    tim1.start();
     */
  }

  INLINE void send_rgb(RGB16 color)
  {
    NT_PORT.out(color.rgb);
    NT_WR.clr();
    NT_WR.set();
  }

  INLINE void send_rgb(RGB16 color, int32_t len)
  {
    NT_PORT.out(color.rgb);
    while (len--) {
      NT_WR.clr();
      NT_WR.set();
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

    uint32_t len = (x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1);
    NT_PORT.out(color.rgb);

  #ifndef WR_FORSED
    while (len--) {
      NT_WR.clr();
      NT_WR.set();
    }
  #else

  #ifdef CH32V20x_D6
    if (len > 26) {  // Порог эффективности
      NT_WR.init(GP_Timer | GPO_10MHz);

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

      NT_WR.init(GPO_10MHz);
    }
    else
      while (len--) {
        NT_WR.clr();
        NT_WR.set();
      }
  #endif

  #ifdef MIK32V2

    len <<= 1;
    NT_WR.init(GP_Timer | GPO_2MHz);
    TIMER32_2->CHANNELS[0].CNTRL =
      TIMER32_CH_CNTRL_MODE_PWM_M |
      TIMER32_CH_CNTRL_ENABLE_M;
    T32_1_C;
    while (T32_1 < len);
    TIMER32_2->CHANNELS[0].CNTRL = 0;
    NT_WR.init(GPO_2MHz);

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

  INLINE void select()
  {
    NT_CS.clr();
    flag = 0;
  }
  INLINE void release()
  {
    NT_CS.clr();
  }
  INLINE void send_rgb(RGB color, int32_t len)
  {
    while (len--) send_rgb(color);
  }
  INLINE void set_rgb_format()
  {
    send_command(NT_COLMOD);
    send_byte(0x06);  // 6x6x6 bit
  }

  void send_rgb(RGB18 color)
  {
    static uint16_t half;
    if (flag) {
      NT_PORT.out(color.red | half);
      NT_WR.clr();
      NT_WR.set();
      flag = 0;
      NT_PORT.out(color.rgb);
      NT_WR.clr();
      NT_WR.set();
    }
    else {
      NT_PORT.out(color.rgb >> 8);
      NT_WR.clr();
      NT_WR.set();
      half = color.blue << 8;
      flag = 1;
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB18 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1) + 1) >> 1;
    uint16_t hword = *(uint16_t *)&color.green;
    uint16_t mword = color.red | (color.blue << 8);
    uint16_t lword = *(uint16_t *)&color.blue;

    while (len--) {
      NT_PORT.out(hword);
      NT_WR.clr();
      NT_WR.set();

      NT_PORT.out(mword);
      NT_WR.clr();
      NT_WR.set();

      NT_PORT.out(lword);
      NT_WR.clr();
      NT_WR.set();
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

  INLINE void select()
  {
    NT_CS.clr();
    flag = 0;
  }
  INLINE void release()
  {
    NT_CS.clr();
  }
  INLINE void send_rgb(RGB color, int32_t len)
  {
    while (len--) send_rgb(color);
  }

  INLINE void set_rgb_format()
  {
    send_command(NT_COLMOD);
    send_byte(0x07);  // 8x8x8 bit
  }

  void send_rgb(RGB24 color)
  {
    static uint16_t half;
    if (flag) {
      NT_PORT.out(color.red | half);
      NT_WR.clr();
      NT_WR.set();
      flag = 0;
      NT_PORT.out(color.rgb);
      NT_WR.clr();
      NT_WR.set();
    }
    else {
      NT_PORT.out(color.rgb >> 8);
      NT_WR.clr();
      NT_WR.set();
      half = color.blue << 8;
      flag = 1;
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB24 color)
  {
    select();
    set_addr(x0, y0, x1, y1);
    uint32_t len = ((x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1) + 1) >> 1;
    uint16_t hword = *(uint16_t *)&color.green;
    uint16_t mword = color.red | (color.blue << 8);
    uint16_t lword = *(uint16_t *)&color.blue;

    while (len--) {
      NT_PORT.out(hword);
      NT_WR.clr();
      NT_WR.set();

      NT_PORT.out(mword);
      NT_WR.clr();
      NT_WR.set();

      NT_PORT.out(lword);
      NT_WR.clr();
      NT_WR.set();
    }
    release();
  }
};
