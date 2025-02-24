#include "midi.h"

void MIDI::_send_midi(uint8_t msg, uint8_t data1, uint8_t data2)
{
  static uint8_t status = 0;
  if (msg != status) {
    send_midi(status = msg, data1, data2);
  }
  else
    send_midi(data1, data2);
}

void MIDI::_send_midi(uint8_t msg, uint8_t data)
{
  static uint8_t status = 0;
  if (msg != status) {
    send_midi(status = msg, data);
  }
  send_midi(data);
}

