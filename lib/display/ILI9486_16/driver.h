#pragma once
#include "comon/include.h"
#include "pins.h"

template<typename C>
class ILI9486_16 : public IDriver {
private:
  uint16_t flag;
  ATTR_INLINE void set_rgb_format();

public:
  using RGB = C;
  ATTR_INLINE constexpr int16_t max_x() { return 319; }
  ATTR_INLINE constexpr int16_t max_y() { return 479; }
  ATTR_INLINE  void select() { L_CS(CLR); }
  ATTR_INLINE  void release() { L_CS(SET); }
  ATTR_INLINE void send_rgb(C color, int32_t len) { while (len--) send_rgb(color); }

  void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0)
  {
    // SEL_0(GPIO); SEL_0(OUT); SEL_0(CLR);  // PORT 0.3 -> D9
    L_WR(GPIO); L_RS(GPIO); L_CS(GPIO);
    L_WR(OUT); L_RS(OUT); L_CS(OUT);
    L_WR(CLR); L_RS(CLR); L_CS(SET);
    PAD_CONFIG->PORT_0_CFG = 0;           // PORT 0 -> GPIO
    L_PORT(OUT) | 0xFFFF;                 // PORT 0 -> OUT

    select();
    send_command(SLPOUT);
    delay_ms(30);
    send_config(ILI9486_CONFIG, sizeof(ILI9486_CONFIG));
    send_command(MADCTL);
    send_byte((rotation | 0x08));         // BGR -> RBG
    set_rgb_format();
    send_command(NORON);
    send_command(DISPON);
    release();
  }

  ATTR_INLINE void send_command(uint8_t command)
  {
    L_RS(CLR);
    send_byte(command);
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
    L_PORT(OUTPUT) = (data >> 8);
    L_WR(SET);
    L_WR(CLR);
    L_PORT(OUTPUT) = data;
    L_WR(SET);
    L_WR(CLR);
  }

  // void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  // {
  //   send_command(CASET); send_word(x0); send_word(x1);
  //   send_command(RASET); send_word(y0); send_word(y1);
  //   send_command(RAMWR);
  // }
};

#include "rgb16.tpp"
#include "rgb18.tpp"
