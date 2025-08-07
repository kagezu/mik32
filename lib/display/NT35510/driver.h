#pragma once
#include "comon/include.h"
#include "const.h"
#include "pins.h"
#include "timer.h"

template<typename C>
class NT35510 {
private:
  uint16_t flag;
  ATTR_INLINE void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 479; }
  ATTR_INLINE constexpr int16_t max_y() { return 799; }
  ATTR_INLINE  void select() { NT_CS(CLR); }
  ATTR_INLINE  void release()
  {
    if (flag) send_rgb(0xFFFFFF);
    NT_CS(CLR);
  }

  ATTR_INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }

  void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0)
  {
    SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR);  // PORT 0.3 -> D9
    NT_WR(GPIO); NT_RS(GPIO); NT_CS(GPIO);
    NT_WR(OUT); NT_RS(OUT); NT_CS(OUT);
    NT_WR(CLR); NT_RS(CLR); NT_CS(SET);
    PAD_CONFIG->PORT_0_CFG = 0;           // PORT 0 -> GPIO
    GPIO_0->DIRECTION_OUT = 0xFFFF;
    GPIO_0->CLEAR = 0xFFFF;
    GPIO_0->SET = 0xFFFF;

    select();
    send_command(NT_SWRESET);
    send_config(NT35510_CONFIG_STEP0, sizeof(NT35510_CONFIG_STEP0));
    delay_ms(50);
    send_config(NT35510_CONFIG_STEP1, sizeof(NT35510_CONFIG_STEP1));
    send_command(NT_MADCTL);
    send_byte((rotation | 0x00));         // BGR -> RBG
    set_rgb_format();
    send_command(NT_NORON);
    send_command(NT_SLPOUT);
    delay_ms(30);
    send_command(NT_DISPON);
    release();

    T32_2_PS;
    TIMER32_2->TOP = 1;
    TIMER32_2->CHANNELS[0].OCR = 1;
    T32_2_EN;
    T32_1_PS;
    T32_1_EN;
  }


  ATTR_INLINE void send_command(uint16_t com)
  {
    NT_RS(CLR);
    NT_PORT(OUTPUT) = com;
    NT_WR(SET);
    NT_WR(CLR);
    NT_WR(SET);
    NT_WR(CLR);
    NT_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data)
  {

    NT_PORT(OUTPUT) = data;
    NT_WR(SET);
    NT_WR(CLR);
  }

  ATTR_INLINE void send_word(uint16_t data)
  {
    NT_WR(CLR);
    NT_PORT(OUTPUT) = (data >> 8);
    NT_WR(SET);
    NT_WR(CLR);
    NT_PORT(OUTPUT) = data;
    NT_WR(SET);
    NT_WR(CLR);
  }

  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    send_command(NT_CASET);
    send_byte(x0 >> 8);
    send_command(NT_CASET + 1);
    send_byte(x0);
    send_command(NT_CASET + 2);
    send_byte(x1 >> 8);
    send_command(NT_CASET + 3);
    send_byte(x1);
    send_command(NT_RASET);
    send_byte(y0 >> 8);
    send_command(NT_RASET + 1);
    send_byte(y0);
    send_command(NT_RASET + 2);
    send_byte(y1 >> 8);
    send_command(NT_RASET + 3);
    send_byte(y1);
    send_command(NT_RAMWR);
  }

  void send_config(const uint8_t *config, int16_t size)
  {
    while (size > 0) {
      uint8_t count = *config++;
      uint16_t comand = (uint16_t)(*config++) << 8;
      size -= 2 + count;
      for (int i = 0; i < count; i++) {
        send_command(comand + 1);
        send_byte(*config++);
      }
    }
  }

  void pixel(int16_t x, int16_t y, C color)
  {
    select();
    set_addr(x, y, x, y);
    send_rgb(color);
    send_rgb(color);
    release();
  }
};



////////////////////////////////// RGB16 //////////////////////////////////////

template<>
class NT35510<RGB16> {
public:
  using RGB = RGB16;

#include "base.h"

  void set_rgb_format()
  {
    send_command(NT_COLMOD);
    send_byte(0x05); // 5x6x5 bit
  }

  ATTR_INLINE void send_rgb(RGB16 color)
  {
    NT_PORT(OUTPUT) = color.rgb;
    NT_WR(CLR);
    NT_WR(SET);
  }

  ATTR_INLINE void send_rgb(RGB16 color, int32_t len)
  {
    NT_PORT(OUTPUT) = color.rgb;
    while (len--) {
      NT_WR(CLR);
      NT_WR(SET);
    }
  }

  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    select();
    set_addr(x0, y0, x1, y1);

    uint32_t len = (x1 - x0 + 1) * (uint32_t)(y1 - y0 + 1);
    NT_PORT(OUTPUT) = color.rgb;
    // while (len--) {
    //   NT_WR(CLR);
    //   NT_WR(SET);
    // }

    len <<= 1;
    T32_1_TOP(0);
    NT_WR(TIMER);
    TIMER32_2->CHANNELS[0].CNTRL =
      TIMER32_CH_CNTRL_MODE_PWM_M |
      TIMER32_CH_CNTRL_ENABLE_M;
    T32_1_C;
    while (T32_1 < len);// NT_PORT(OUTPUT) = T32_1 * color.rgb;
    TIMER32_2->CHANNELS[0].CNTRL = 0;
    NT_WR(GPIO);

    release();
  }
};

////////////////////////////////// RGB18 //////////////////////////////////////


////////////////////////////////// RGB24 //////////////////////////////////////



// #include "rgb16.tpp"
// #include "rgb18.tpp"
// #include "rgb24.tpp"
