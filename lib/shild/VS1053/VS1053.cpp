#include "VS1053.h"
#include "rtmidi.h"

#define WAIT_DREQ     while (!X_DREQ(GET));

extern SPI spi;

// Инициализация
//////////////////////////////////////////////////////////////

void VS1053::init()
{
  X_DREQ(GPIO); X_CS(GPIO); X_DCS(GPIO);
  X_DREQ(IN);  X_CS(OUT);  X_DCS(OUT);
  X_CS(SET);  X_DCS(SET);

  spi.begin(_init);
  write_register(SCI_CLOCKF, SM_RESET);
  WAIT_DREQ;
  write_register(SCI_CLOCKF, SC_MULT | SC_ADD | SC_FREQ);
  spi.end();
  WAIT_DREQ;
  set_master(SCI_VOL_DEFAULT);
  load_patch(rtmidi);
}

void VS1053::rt() { load_patch(rtmidi); }

void VS1053::send(uint8_t *ptr, uint8_t size)
{
  X_DCS(CLR);
  spi.begin(_write);
  while (size--) {
    spi.transfer16(*ptr++);
  }
  X_DCS(SET);
  spi.end();
}

// Управление
////////////////////////////////////////////////////////////////////////








// Трансфер функции
//////////////////////////////////////////////////////////////////////////

uint16_t VS1053::read_register(uint8_t addr)
{
  uint16_t result;
  X_CS(CLR);
  spi.transfer(SCI_READ);
  spi.transfer(addr);
  result = spi.transfer16(0xFFFF);
  X_CS(SET);
  return result;
}

void VS1053::write_register(uint8_t addr, uint16_t data)
{
  X_CS(CLR);
  spi.transfer(SCI_WRITE);
  spi.transfer(addr);
  spi.transfer16(data);
  WAIT_DREQ;
  X_CS(SET);
}

void VS1053::send_midi(uint8_t msg)
{
  X_DCS(CLR);
  // spi.transfer(SCI_MIDI);
  spi.transfer16(msg);
  X_DCS(SET);
}

void VS1053::send_midi(uint8_t msg, uint8_t data)
{
  X_DCS(CLR);
  // spi.transfer(SCI_MIDI);
  spi.transfer16(msg);
  // spi.transfer(SCI_MIDI);
  spi.transfer(data);
  // spi.wait();
  // WAIT_DREQ;
  X_DCS(SET);
}

void VS1053::send_midi(uint8_t msg, uint8_t data1, uint8_t data2)
{
  X_DCS(CLR);
  // spi.transfer(SCI_MIDI);
  spi.transfer16(msg);
  // spi.transfer(SCI_MIDI);
  spi.transfer16(data1);
  // spi.transfer(SCI_MIDI);
  spi.transfer16(data2);
  // spi.wait();
  // WAIT_DREQ;
  X_DCS(SET);
}

// Приватные функции
/////////////////////////////////////////////////////////////

void VS1053::load_patch(const uint16_t *data)
{
  uint16_t addr, val, rep;
  spi.begin(_write);
  while (true) {
    addr = pgm_read_word(data++);
    if (addr == END_PATCH) return;
    rep = pgm_read_word(data++);

    if (rep & 0x8000U) {  // Заполняем одним значением
      rep &= 0x7FFF;
      val = pgm_read_word(data++);
      while (rep--) write_register(addr, val);
    }
    else                  // Копируем последовательность
      while (rep--) write_register(addr, pgm_read_word(data++));
  }
  spi.end();
}

uint8_t VS1053::sum_vol(char vol_ch)
{
  int16_t vol = (int16_t)_vol_master + vol_ch;
  return vol < 0 ? 0 : vol>0xfe ? 0xfe : vol;
}

void VS1053::set_volume()
{
  uint16_t volume;
  spi.begin(_write);
  volume = sum_vol(_vol_right);
  volume |= sum_vol(_vol_left) << 8;
  write_register(SCI_VOL, volume);
  spi.end();
}

void VS1053::get_volume()
{
  uint16_t volume;
  spi.begin(_read);
  volume = read_register(SCI_VOL);
  _vol_master = ((uint16_t)to_byte(volume, 0) + to_byte(volume, 1)) >> 1;
  _vol_right = _vol_master - to_byte(volume, 0);
  _vol_left = _vol_master - to_byte(volume, 1);
  spi.end();
}
