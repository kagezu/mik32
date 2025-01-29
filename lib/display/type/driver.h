#pragma once
#include <core.h>

template<typename C>
class IDriver {
public:
  virtual void init() = 0;
  virtual void area(uint16_t, uint16_t, uint16_t, uint16_t, C) = 0;

protected:
  virtual void send_command(uint8_t) = 0;
  virtual void set_addr(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
  virtual void send_zero() = 0;
  virtual void send_byte(uint8_t) = 0;
  virtual void send_rgb(C) = 0;

  void send_config(const uint8_t *config, uint8_t size)
  {



    // while (size) {
    //   uint8_t data, comand = *config++;
    //   size -= 2;
    //   send_command(comand);
    //   while ((data = *config++) != 0xFF) {
    //     send_byte(data);
    //     size--;
    //   }
    // }



    while (size) {
      uint8_t data, comand = pgm_read_byte(config++);
      size -= 2;
      send_command(comand);
      while ((data = pgm_read_byte(config++)) != 0xFF) {
        send_byte(data);
        size--;
      }
    }





  }
};
