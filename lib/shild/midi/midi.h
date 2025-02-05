#pragma once
#include "midi-types.h"
#include "midi-text.h"

#define MIDI_VELOCITY_DEFAULT   0x40

class MIDI {
public:
  void note_off(uint8_t note, uint8_t ch = 0, uint8_t velocity = MIDI_VELOCITY_DEFAULT) { _send_midi(NoteOff | ch, note, velocity); }
  void note_on(uint8_t note, uint8_t ch = 0, uint8_t velocity = MIDI_VELOCITY_DEFAULT) { _send_midi(NoteOn | ch, note, velocity); }
  void key_press(uint8_t note, uint8_t press, uint8_t ch = 0) { _send_midi(KeyPressure | ch, note, press); }
  void ctrl_change(MIDI_Control ctrl, uint8_t value, uint8_t ch = 0) { _send_midi(ControlChange | ch, ctrl, value); }
  void pgm_change(uint8_t pgm, uint8_t ch = 0) { _pgm = pgm; _send_midi(ProgramChange | ch, pgm); }
  void ch_press(uint8_t press, uint8_t ch = 0) { _send_midi(ChannelPressure | ch, press); }
  void pitch_bend(uint8_t lsb = 0, uint8_t msb = 0x20, uint8_t ch = 0) { _send_midi(PitchBend | ch, lsb, msb); }

public:
  const char *get_pgm_text() { return MIDI_pgm[_pgm]; }

protected:
  virtual void send_midi(uint8_t) = 0;
  virtual void send_midi(uint8_t, uint8_t) = 0;
  virtual void send_midi(uint8_t, uint8_t, uint8_t) = 0;

private:
  uint8_t _pgm = 0;
  uint8_t _run_status = 0;
  void _send_midi(uint8_t, uint8_t, uint8_t);
  void _send_midi(uint8_t, uint8_t);
};
