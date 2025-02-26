#pragma once
#include "VS1053/VS1053.h"

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
    length = (uint32_t)pgm_read_dword(&(mtrk->length_section));
    ptr = pgm + sizeof(MIDI_MTrk);
    pgm = ptr + length;
    set_time();
    timer = 0;
    return pgm;
  }

  uint8_t *next(uint8_t *buffer)
  {
    while (timer == 0 && length) {
      uint8_t event = read();
      if (event == 0xFF) {
        event = read(); // Считываем номер мета-события.
        event = read(); // Считываем длину.
        for (uint8_t loop = 0; loop < event; loop++) read();
      }
      else switch (event & 0xF0) {
        // Перебираем события первого уровня.

        case 0x80: // Снять клавишу.
        case 0x90:   // Нажать клавишу.
        case 0xA0:  // Сменить силу нажатия клавишы.
        case 0xE0:  // Вращения звуковысотного колеса.
          *buffer++ = event;
          *buffer++ = read();
          *buffer++ = read();
          break;

          // Если 2-х байтовая комманда.
        case 0xB0:
          event = read(); // Читаем саму команду.
          switch (event) // Смотрим команды второго уровня.
          {
            default: // Для определения новых комманд (не описаных).
              event = read(); // Считываем параметр какой-то неизвестной функции.
              break;
          }
          break;

        case 0xC0:// Считываем номер программы.
        case 0xD0:   // Сила канала.
          *buffer++ = event;
          *buffer++ = read();
          break;
      }
      if (length) set_time();
    }
    // timer--;
    timer = 0;
    return buffer;
  }

private:
  uint8_t read() { length--; return pgm_read_byte(ptr++); }
  void set_time()
  {
    uint8_t  tmp;
    timer = 0;
    do {
      tmp = read();
      timer <<= 7;
      timer |= (byte)(tmp & (0x7F));
    } while ((tmp & (1 << 7)) != 0);
  }

  uint8_t *ptr;
  uint32_t length;
  uint32_t timer;
};

class MIDI_Play {
public:
  MIDI_Play(VS1053 *midi) :_MIDI(midi) {}
  void open(const uint8_t *ptr) { file = (uint8_t *)ptr; }

  void play()
  {
    uint8_t *ptr = file;
    ptr = read(ptr, &header, sizeof(header));
    header.setting_time = (50000 / header.setting_time) * 10;
    ptr = file + sizeof(MIDI_MThd);
    for (uint8_t i = 0; i < header.channels; i++) ptr = channel[i].init(ptr);
  }

  void tick()
  {
    delay_ms(100);
    uint8_t *buf = buffer;
    for (uint8_t i = 0; i < header.channels; i++) {
      buf = channel[i].next(buf);
    }
    uint8_t size = buf - buffer;
    if (size) _MIDI->send(buffer, size);
  }

private:
  uint8_t *read(uint8_t *src, void *tgt, size_t size)
  {
    uint8_t *ptr = (uint8_t *)tgt;
    while (size--) *ptr++ = pgm_read_byte(src++);
    return src;
  }

  uint8_t buffer[528];

  VS1053 *_MIDI;
  MIDI_MThd header;
  uint8_t *file;
  MIDI_channel channel[28];
};
