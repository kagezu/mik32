#pragma once
#include "comon/include.h"
#include "const.h"
#include "pins.h"

template<typename C>
class NT35510 {
private:
  uint16_t flag;
  ATTR_INLINE void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 479; }
  ATTR_INLINE constexpr int16_t max_y() { return 799; }
  ATTR_INLINE  void select() { L_CS(CLR); }
  ATTR_INLINE  void release()
  {
    if (flag) send_rgb(0xFFFFFF);
    L_CS(CLR);
  }

  ATTR_INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }

  void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0)
  {
    SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR);  // PORT 0.3 -> D9
    L_WR(GPIO); L_RS(GPIO); L_CS(GPIO);
    L_WR(OUT); L_RS(OUT); L_CS(OUT);
    L_WR(CLR); L_RS(CLR); L_CS(SET);
    PAD_CONFIG->PORT_0_CFG = 0;           // PORT 0 -> GPIO
    // PAD_CONFIG->PORT_0_PUPD = 0xAAAA;
    GPIO_0->DIRECTION_OUT = 0xFFFF;
    GPIO_0->SET = 0xFFFF;


    delay_ms(50);
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
  }


  ATTR_INLINE void send_command(uint16_t com)
  {
    L_RS(CLR);
    L_PORT(OUTPUT) = com;
    L_WR(SET);
    L_WR(CLR);
    L_WR(SET);
    L_WR(CLR);
    L_RS(SET);
  }

  ATTR_INLINE void send_byte(uint8_t data)
  {

    L_PORT(OUTPUT) = data;
    L_WR(SET);
    L_WR(CLR);
  }

  ATTR_INLINE void send_word(uint16_t data)
  {
    L_WR(CLR);
    L_PORT(OUTPUT) = (data >> 8);
    L_WR(SET);
    L_WR(CLR);
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    L_WR(CLR);
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
};

#include "rgb16.tpp"
#include "rgb18.tpp"
#include "rgb24.tpp"
