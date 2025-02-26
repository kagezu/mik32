// #include "VS1053.h"
#pragma once
#include "pins.h"
#include "midi/midi.h"
#include "const.h"
#include "spi/SPI.h"

class VS1053 : public MIDI {
public:
  VS1053() { _init.init(SCI_FQ_INIT); _write.init(SCI_FQ_WRITE); _read.init(SCI_FQ_READ); }
  void init();
  void rt();
  void send(uint8_t *, uint8_t);

  // Управление громкостью

  void set_left(uint8_t  left) { _vol_left = left; set_volume(); }
  void set_right(uint8_t  right) { _vol_right = right; set_volume(); }
  void set_master(uint8_t vol) { _vol_master = vol; set_volume(); }

  uint8_t get_master() { return _vol_master; }
  uint8_t get_left() { return _vol_left; }
  uint8_t get_right() { return _vol_right; }

  // protected:

  void send_midi(uint8_t);
  void send_midi(uint8_t, uint8_t);
  void send_midi(uint8_t, uint8_t, uint8_t);

  // private:
  void load_patch(const uint16_t *);
  void write_register(uint8_t, uint16_t);
  uint16_t read_register(uint8_t);
  void get_volume();
  void set_volume();
  uint8_t sum_vol(char vol_ch);

  SPI_Settings _init;
  SPI_Settings _read;
  SPI_Settings _write;

  uint8_t _vol_master;
  uint8_t _vol_left = 0;
  uint8_t _vol_right = 0;
};
