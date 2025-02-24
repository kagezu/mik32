#pragma once
#include "midi/midi.h"

struct MIDI_MThd {
  char name_section[4];           // Имя раздела. Должно быть "MThd".
  uint32_t length_section;        // Длинна блока, 4 байта. Должно быть 0x6;
  uint16_t mode;                  // Режим MIDI файла: 0, 1 или 2. 
  uint16_t channels;              // Количество каналов. 
  uint16_t setting_time;          // Параметры тактирования.
};

struct MIDI_MTrk {
  char name_section[4];           // Имя раздела. Должно быть "MTrk".
  uint32_t length_section;        // Длинна блока, 4 байта.
};

class MIDI_channel {
public:
  uint8_t *init(uint8_t *pgm)
  {
    MIDI_MTrk *mtrk = (MIDI_MTrk *)pgm;
    pgm += sizeof(MIDI_MTrk);
    ptr = pgm;
    length = (uint32_t)pgm_read_dword(&(mtrk->length_section));
    timer = 0;
    ptr += length;
    return ptr;
  }
  uint8_t read() { if (length) { length--; return pgm_read_byte(ptr++); } return 0; }
  uint8_t is_time()
  {
    if (timer == 0) return 1;
    timer--;
    return 0;
  }
  void next()
  {
    uint8_t  buffer;
    do {
      buffer = read();
      timer <<= 7;
      timer |= (byte)(buffer & (0x7F));
    } while ((buffer & (1 << 7)) != 0);
  }

  // private:
  uint8_t *ptr;
  uint32_t length;
  uint16_t timer;
};

class MIDI_Play {
public:
  MIDI_Play(MIDI *midi) :_MIDI(midi) {}
  void open(const uint8_t *ptr) { file = (uint8_t *)ptr; }

  void play()
  {
    uint8_t *ptr = file;
    ptr = read(ptr, &header, sizeof(header));
    header.setting_time = 500000u / header.setting_time;
    ptr = file + sizeof(MIDI_MThd);
    for (uint8_t i = 0; i < header.channels; i++) { ptr = channel[i].init(ptr); channel[i].next(); }
  }

  uint8_t step()
  {
    delay_us(header.setting_time);
    uint16_t end = 0;
    for (uint8_t i = 0; i < header.channels; i++) {
      if (channel[i].is_time()) {
        uint8_t event = channel[i].read();
        uint8_t data1 = channel[i].read();
        if (event >= 192 && event <= 223) {
          uint8_t data2 = channel[i].read();
          _MIDI->send_midi(event, data1, data2);
        }
        else {
          _MIDI->send_midi(event, data1);
        }
        end |= channel[i].length;
      }
    }
    return end | (end >> 8);
  }

private:
  uint8_t *read(uint8_t *src, void *tgt, size_t size)
  {
    uint8_t *ptr = (uint8_t *)tgt;
    while (size--) *ptr++ = pgm_read_byte(src++);
    return src;
  }

  MIDI *_MIDI;
  MIDI_MThd header;
  uint8_t *file;
  MIDI_channel channel[8];
};
