#pragma once
#include "SPI.h"
#include "pins.h"
#include "comon/include.h"

#define SPI_THR_2     7
#define SPI_THR_3     6

template<typename S, typename C>
class ST7735 : public IDriver {
private:
  ATTR_INLINE void set_rgb_format();
  uint8_t flag = 0;

public:
  using RGB = C;
  S spi;
  SPIConf spi_conf;

  ATTR_INLINE constexpr int16_t max_x() { return 127; }
  ATTR_INLINE constexpr int16_t max_y() { return 159; }
  ATTR_INLINE void select() { spi.begin(spi_conf); ST_SPI_CS(CLR); }
  ATTR_INLINE void release() { spi.end(); ST_SPI_CS(SET); }
  ATTR_INLINE void send_rgb(C color, uint8_t len) { while (len--)send_rgb(color); }
  ATTR_INLINE void send_byte(uint8_t data) { spi.send(data); spi.wait_idle(); }
  ATTR_INLINE void send_word(uint16_t data) { spi.send16(data); spi.wait_idle(); }

  ATTR_INLINE void send_rgb(C color);
  void area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, C color);

  void init(uint8_t rotation = 0)
  {
    ST_SPI_RST(OUT); ST_SPI_RST(CLR);
    ST_SPI_CS(OUT); ST_SPI_CS(SET);
    ST_SPI_RS(OUT); ST_SPI_RS(SET);
    ST_SPI_RST(SET);

    // Установка порога для буфера
    spi_conf.thr(sizeof(C) == 2 ? SPI_THR_2 : SPI_THR_3);
    spi_conf.mode(SPI_MODE0); // Настройка для инициализации

    select();
    send_command(SWRESET);
    delay_ms(50);
    send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));
    send_command(MADCTL);
    send_byte(rotation);
    set_rgb_format();
    send_command(DISPON);
    release();

    spi_conf.mode(SPI_MODE3);// Ускоряет ещё на 11,7% для MIK32
  }

protected:
  ATTR_INLINE void send_command(uint8_t command)
  {
    ST_SPI_RS(CLR); // Запись команды
    send_byte(command);
    ST_SPI_RS(SET); // Запись данных
  }

  //   void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  //   {
  //     send_command(CASET);
  //     spi.send16(x0);
  //     spi.wait();
  //     spi.send16(x1);
  //     spi.wait_idle();

  //     send_command(RASET);
  //     spi.send16(y0);
  //     spi.wait();
  //     spi.send16(y1);
  //     spi.wait_idle();

  //     send_command(RAMWR);
  //   }
};

#include "rgb12.tpp"
#include "rgb16.tpp"
#include "rgb18.tpp"
