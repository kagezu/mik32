// #include "VS1053.h"
#pragma once
#include "pins.h"
#include "SPI.h"
#include "midi/midi.h"

// SC_MULT Множитель частоты  SPI READ / WRITE
// 0 0x0000 × 1.0   = 12  1,7 /  3
// 1 0x2000 × 2.0   = 24  3,4 /  6
// 2 0x4000 × 2.5   = 30  4,2 /  7,5
// 3 0x6000 × 3.0   = 36  5,1 /  9
// 4 0x8000 × 3.5   = 42  6,0 / 10,5
// 5 0xA000 × 4.0   = 48  6,8 / 12
// 6 0xC000 × 4.5   = 54  7,7 / 13,5   MAX CPU
// 7 0xE000 × 5.0   = 60  8,5 / 15

#define SC_MULT           0x8000

//   SC_ADD   XTALI
// 0 0x0000 + 0
// 1 0x0800 + 1.0
// 2 0x1000 + 1.5
// 3 0x1800 + 2.0

#define SC_ADD            0x0800

// Частота кварца в кГц
#define SC_FQ_CRYSTAL     12000
#define SC_FREQ           ((SC_FQ_CRYSTAL - 8000) >> 2)

// Константы
////////////////////////////////////////////////////////////////////////////////


// Инициализация SPI, в килогерцах
#define SCI_FQ_INIT       (SC_FQ_CRYSTAL >> 3)
#define SCI_FQ_WRITE      (((((SC_MULT >> 13) + 3) * SC_FQ_CRYSTAL) >> 1) / 4)
#define SCI_FQ_READ       (((((SC_MULT >> 13) + 3) * SC_FQ_CRYSTAL) >> 1) / 7)

// -20 dB
#define SCI_VOL_DEFAULT   20



#define SCI_MIDI          0x0
#define SCI_WRITE         0x2
#define SCI_READ          0x3

// datasheet стр 37

#define SCI_MODE          0x0
#define SCI_STATUS        0x1
#define SCI_BASS          0x2
#define SCI_CLOCKF        0x3
#define SCI_DECODE_TIME   0x4
#define SCI_AUDATA        0x5
#define SCI_WRAM          0x6
#define SCI_WRAMADDR      0x7
#define SCI_HDAT0         0x8
#define SCI_HDAT1         0x9
#define SCI_AIADDR        0xA
#define SCI_VOL           0xB
#define SCI_AICTRL0       0xC
#define SCI_AICTRL1       0xD
#define SCI_AICTRL2       0xE
#define SCI_AICTRL3       0xF

#define SM_DIFF           0x0001
#define SM_LAYER12        0x0002
#define SM_RESET          0x0004
#define SM_CANCEL         0x0008
#define SM_EARSPEAKER_LO  0x0010
#define SM_TESTS          0x0020
#define SM_STREAM         0x0040
#define SM_EARSPEAKER_HI  0x0080
#define SM_DACT           0x0100
#define SM_SDIORD         0x0200
#define SM_SDISHARE       0x0400
#define SM_SDINEW         0x0800
#define SM_ADPCM          0x1000
#define SM_PAUSE          0x2000
#define SM_LINE1          0x4000
#define SM_CLK_RANGE      0x8000

#define END_PATCH         0xFFFF

class VS1053 : public MIDI {
public:
  void init();

  // Управление громкостью

  void set_left(uint8_t  left) { _vol_left = left; set_volume(); }
  void set_right(uint8_t  right) { _vol_right = right; set_volume(); }
  void set_master(uint8_t vol) { _vol_master = vol; set_volume(); }

  uint8_t get_master() { return _vol_master; }
  uint8_t get_left() { return _vol_left; }
  uint8_t get_right() { return _vol_right; }

protected:
  uint8_t _vol_master;
  uint8_t _vol_left = 0;
  uint8_t _vol_right = 0;

  void send_midi(uint8_t);
  void send_midi(uint8_t, uint8_t);
  void send_midi(uint8_t, uint8_t, uint8_t);

private:
  SPI_Master SPI;

  void load_patch(const uint16_t *);
  void write_register(uint8_t, uint16_t);
  uint16_t read_register(uint8_t);
  void get_volume();
  void set_volume();
  uint8_t sum_vol(char vol_ch);
};
