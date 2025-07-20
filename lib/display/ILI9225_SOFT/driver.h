#pragma once
#include "pins.h"
#include "const.h"
#include "type/include.h"

template< const int R = R_0>
class ILI9225_SOFT {
public:
  using RGB = RGB16; // Только 16 бит
  ATTR_INLINE constexpr int16_t max_x() { return R & EX_X_Y ? 219 : 175; }
  ATTR_INLINE constexpr int16_t max_y() { return R & EX_X_Y ? 175 : 219; }

  void init()
  {
    ILI_SOFT_SCK(GPIO); ILI_SOFT_SDA(GPIO); ILI_SOFT_RST(GPIO); ILI_SOFT_CS(GPIO); ILI_SOFT_RS(GPIO);
    ILI_SOFT_SCK(OUT); ILI_SOFT_SDA(OUT); ILI_SOFT_RST(OUT); ILI_SOFT_CS(OUT); ILI_SOFT_RS(OUT);
    ILI_SOFT_CS(SET); ILI_SOFT_RS(SET);

    ILI_SOFT_RST(CLR);     // Аппаратный сброс
    delay_ms(2);
    ILI_SOFT_RST(SET);
    delay_ms(50);
    select();

    send_command(ILI9225_POWER_CTRL1); send_word(0x0000); // Set SAP,DSTB,STB
    send_command(ILI9225_POWER_CTRL2); send_word(0x0000); // Set APON,PON,AON,VCI1EN,VC
    send_command(ILI9225_POWER_CTRL3); send_word(0x0000); // Set BT,DC1,DC2,DC3
    send_command(ILI9225_POWER_CTRL4); send_word(0x0000); // Set GVDD
    send_command(ILI9225_POWER_CTRL5); send_word(0x0000); // Set VCOMH/VCOML voltage

    delay_ms(50);

    send_command(ILI9225_POWER_CTRL2); send_word(0x0018); // Set APON,PON,AON,VCI1EN,VC
    send_command(ILI9225_POWER_CTRL3); send_word(0x6121); // Set BT,DC1,DC2,DC3
    send_command(ILI9225_POWER_CTRL4); send_word(0x006F); // Set GVDD   /*007F 0088 */
    send_command(ILI9225_POWER_CTRL5); send_word(0x495F); // Set VCOMH/VCOML voltage
    send_command(ILI9225_POWER_CTRL1); send_word(0x0800); // Set SAP,DSTB,STB

    delay_ms(10);

    send_command(ILI9225_POWER_CTRL2); send_word(0x103B); // Set APON,PON,AON,VCI1EN,VC
    send_command(ILI9225_LCD_AC_DRIVING_CTRL); send_word(0x0100);

    constexpr int NL_220 = 0x001C;
    constexpr int     SS = 0x0100;
    constexpr int     GS = 0x0200;

    send_command(ILI9225_DRIVER_OUTPUT_CTRL); // Отражения по осям
    send_word(
      NL_220 |
      (R & FLIP_X ? 0 : SS) |
      (R & FLIP_Y ? GS : 0)
    );
    send_command(ILI9225_ENTRY_MODE);
    send_word(0x1000 | 0x30 | ((R & EX_X_Y) >> 2));// Обмен осями



    send_command(ILI9225_DISP_CTRL1); send_word(0x0000); // Выключить дисплей
    send_command(ILI9225_BLANK_PERIOD_CTRL1); send_word(0x0808);
    send_command(ILI9225_FRAME_CYCLE_CTRL); send_word(0x1100);

    send_command(ILI9225_INTERFACE_CTRL); send_word(0x0000);
    send_command(ILI9225_OSC_CTRL); send_word(0x0D01);
    send_command(ILI9225_VCI_RECYCLING); send_word(0x0020);

    send_command(ILI9225_GATE_SCAN_CTRL); send_word(0x0000);
    send_command(ILI9225_VERTICAL_SCROLL_CTRL1); send_word(0x00DB);
    send_command(ILI9225_VERTICAL_SCROLL_CTRL2); send_word(0x0000);
    send_command(ILI9225_VERTICAL_SCROLL_CTRL3); send_word(0x0000);
    send_command(ILI9225_PARTIAL_DRIVING_POS1); send_word(0x00DB);
    send_command(ILI9225_PARTIAL_DRIVING_POS2); send_word(0x0000);
    send_command(ILI9225_HORIZONTAL_WINDOW_ADDR1); send_word(0x00AF);
    send_command(ILI9225_HORIZONTAL_WINDOW_ADDR2); send_word(0x0000);
    send_command(ILI9225_VERTICAL_WINDOW_ADDR1); send_word(0x00DB);
    send_command(ILI9225_VERTICAL_WINDOW_ADDR2); send_word(0x0000);

    send_command(ILI9225_GAMMA_CTRL1); send_word(0x0000);
    send_command(ILI9225_GAMMA_CTRL2); send_word(0x0808);
    send_command(ILI9225_GAMMA_CTRL3); send_word(0x080A);
    send_command(ILI9225_GAMMA_CTRL4); send_word(0x000A);
    send_command(ILI9225_GAMMA_CTRL5); send_word(0x0A08);
    send_command(ILI9225_GAMMA_CTRL6); send_word(0x0808);
    send_command(ILI9225_GAMMA_CTRL7); send_word(0x0000);
    send_command(ILI9225_GAMMA_CTRL8); send_word(0x0A00);
    send_command(ILI9225_GAMMA_CTRL9); send_word(0x0710);
    send_command(ILI9225_GAMMA_CTRL10); send_word(0x0710);

    send_command(ILI9225_DISP_CTRL1); send_word(0x1017);// Включить дисплей

    release();
  }

protected:
  ATTR_INLINE void select() { ILI_SOFT_CS(CLR); }
  ATTR_INLINE void release() { ILI_SOFT_SCK(CLR); ILI_SOFT_CS(SET); }

  void send_command(uint16_t command)
  {
    ILI_SOFT_RS(CLR); // Запись команды
    send_word(command);
    ILI_SOFT_RS(SET); // Запись данных
  }

  void send_byte(uint8_t _a) {}

  ATTR_NOINLINE void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
    // Обмен осями
  #ifdef __AVR__
    if (R & EX_X_Y) {
    #else
    if constexpr (R & EX_X_Y) {
    #endif
      send_command(ILI9225_HORIZONTAL_WINDOW_ADDR1); send_word(y1);
      send_command(ILI9225_HORIZONTAL_WINDOW_ADDR2); send_word(y0);
      send_command(ILI9225_VERTICAL_WINDOW_ADDR1); send_word(x1);
      send_command(ILI9225_VERTICAL_WINDOW_ADDR2); send_word(x0);
      send_command(ILI9225_RAM_ADDR_SET1); send_word(y0);
      send_command(ILI9225_RAM_ADDR_SET2); send_word(x0);
    }
    else {
      send_command(ILI9225_HORIZONTAL_WINDOW_ADDR1); send_word(x1);
      send_command(ILI9225_HORIZONTAL_WINDOW_ADDR2); send_word(x0);
      send_command(ILI9225_VERTICAL_WINDOW_ADDR1); send_word(y1);
      send_command(ILI9225_VERTICAL_WINDOW_ADDR2); send_word(y0);
      send_command(ILI9225_RAM_ADDR_SET1); send_word(x0);
      send_command(ILI9225_RAM_ADDR_SET2); send_word(y0);
    }

    send_command(ILI9225_GRAM_DATA_REG);
  }

  ATTR_INLINE void send_word(uint16_t data)
  {
    for (uint16_t mask = 0x8000; mask > 0; mask >>= 1) {
      if (data & mask) ILI_SOFT_SDA(SET);
      else ILI_SOFT_SDA(CLR);
      ILI_SOFT_SCK(SET);
      ILI_SOFT_SCK(CLR);
    }
  }

  ATTR_INLINE void send_rgb(RGB16 color, uint8_t len) { while (len--)send_rgb(color); }
  ATTR_NOINLINE void send_rgb(RGB16 color)
  {
    uint16_t rgb = color.rgb;

  #ifdef __AVR__
    uint8_t d0 = ILI_SOFT_SCK(OUTPUT) & ~(ILI_SOFT_SDA(MASK) | ILI_SOFT_SCK(MASK));
    uint8_t d1 = (ILI_SOFT_SCK(OUTPUT) | ILI_SOFT_SDA(MASK)) & ~ILI_SOFT_SCK(MASK);
    uint8_t s0 = (ILI_SOFT_SCK(OUTPUT) & ~ILI_SOFT_SDA(MASK)) | ILI_SOFT_SCK(MASK);

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;
    ILI_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
    ILI_SOFT_SCK(OUTPUT) = s0;

    ILI_SOFT_SCK(CLR);
  #else

    int mask = 0x8000;
    while (mask) {
      if (rgb & mask)  ILI_SOFT_SDA(SET);
      else  ILI_SOFT_SDA(CLR);
      ILI_SOFT_SCK(SET);
      ILI_SOFT_SCK(CLR);
      mask >>= 1;
    }
  #endif
  }

  ATTR_NOINLINE void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB16 color)
  {
    uint16_t rgb = color.rgb;

    ILI_SOFT_CS(CLR);
    set_addr(x0, y0, x1, y1);
    uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  #ifdef __AVR__
    uint8_t d0 = ILI_SOFT_SCK(OUTPUT) & ~(ILI_SOFT_SDA(MASK) | ILI_SOFT_SCK(MASK));
    uint8_t d1 = (ILI_SOFT_SCK(OUTPUT) | ILI_SOFT_SDA(MASK)) & ~ILI_SOFT_SCK(MASK);
    uint8_t s0 = (ILI_SOFT_SCK(OUTPUT) & ~ILI_SOFT_SDA(MASK)) | ILI_SOFT_SCK(MASK);

    // Дублирование кода намеренно, так как оптимизация ускоряет тут выполнение в 2 раза
    while (len--) {
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x8000 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x4000 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x2000 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x1000 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;

      ILI_SOFT_SCK(OUTPUT) = rgb & 0x800 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x400 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x200 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x100 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;

      ILI_SOFT_SCK(OUTPUT) = rgb & 0x80 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x40 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x20 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x10 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;

      ILI_SOFT_SCK(OUTPUT) = rgb & 0x8 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x4 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x2 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
      ILI_SOFT_SCK(OUTPUT) = rgb & 0x1 ? d1 : d0;
      ILI_SOFT_SCK(OUTPUT) = s0;
    }
  #else

    while (len--) {
      int mask = 0x8000;
      while (mask) {
        if (rgb & mask)  ILI_SOFT_SDA(SET);
        else  ILI_SOFT_SDA(CLR);
        ILI_SOFT_SCK(SET);
        ILI_SOFT_SCK(CLR);
        mask >>= 1;
      }
    }
  #endif
    ILI_SOFT_SCK(CLR);
    ILI_SOFT_CS(SET);
  }
};
