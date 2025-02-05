#include "VS1053.h"
#include "rtmidi.h"

#define WAIT_DREQ     while (!X_DREQ(GET));

// Инициализация
//////////////////////////////////////////////////////////////
void VS1053::init()
{
  X_DREQ(GPIO); X_CS(GPIO); X_DCS(GPIO); X_RESET(GPIO);
  X_DREQ(IN);  X_CS(OUT);  X_DCS(OUT);  X_RESET(OUT);
  X_CS(SET);  X_DCS(SET);  X_RESET(CLR);
  delay_ms(10);
  X_RESET(SET); // Отпускаем сброс
  SPI.init(SCI_FQ_INIT);
  WAIT_DREQ;
  write_register(SCI_CLOCKF, SC_MULT | SC_ADD | SC_FREQ);
  SPI.init(SCI_FQ_WRITE); // Ускоряем SPI
  set_master(SCI_VOL_DEFAULT);
  load_patch(rtmidi);
}



// Управление
////////////////////////////////////////////////////////////////////////








// Трансфер функции
//////////////////////////////////////////////////////////////////////////

uint16_t VS1053::read_register(uint8_t addr)
{
  uint16_t result;
  cli();
  X_CS(CLR);
  SPI.send(SCI_READ);
  SPI.send(addr);
  result = SPI.transfer16(0xFFFF);
  WAIT_DREQ;
  X_CS(SET);
  sei();
  return result;
}

void VS1053::write_register(uint8_t addr, uint16_t data)
{
  X_CS(CLR);
  SPI.send(SCI_WRITE);
  SPI.send(addr);
  SPI.send16(data);
  WAIT_DREQ;
  X_CS(SET);
}

void VS1053::send_midi(uint8_t msg)
{
  X_DCS(CLR);
  SPI.send16(msg);
  X_DCS(SET);
}

void VS1053::send_midi(uint8_t msg, uint8_t data)
{
  X_DCS(CLR);
  SPI.send16(msg);
  SPI.send16(data);
  X_DCS(SET);
}

void VS1053::send_midi(uint8_t msg, uint8_t data1, uint8_t data2)
{
  X_DCS(CLR);
  SPI.send16(msg);
  SPI.send16(data1);
  SPI.send16(data2);
  X_DCS(SET);
}

// Приватные функции
/////////////////////////////////////////////////////////////

void VS1053::load_patch(const uint16_t *data)
{
  uint16_t addr, val, rep;
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
}

uint8_t VS1053::sum_vol(char vol_ch)
{
  int16_t vol = (int16_t)_vol_master + vol_ch;
  return vol < 0 ? 0 : vol>0xfe ? 0xfe : vol;
}

void VS1053::set_volume()
{
  dbyte volume;
  volume.low = sum_vol(_vol_right);
  volume.high = sum_vol(_vol_left);
  write_register(SCI_VOL, volume.word);
}

void VS1053::get_volume()
{
  dbyte vol;
  vol.word = read_register(SCI_VOL);
  _vol_master = ((uint16_t)vol.low + vol.high) >> 2;
  _vol_right = _vol_master - vol.low;
  _vol_left = _vol_master - vol.high;
}
