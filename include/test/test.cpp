#include "config.h"
#include "VS1053/VS1053.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"
#include "ernani.mid.h"
#include "midi-play/midi-play.h"

Display lcd;
VS1053 midi;

int main(void)
{
  MIDI_Play play(&midi);
  init_system();
  SPI.begin();
  lcd.init();
  midi.init();
  lcd.background(RGB(0, 16, 32));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);

  midi.pgm_change(30);

  lcd.printf(P("\f  Started"));

  play.open(ernani);
  play.play();
  while (true) {
    if (!play.step()) play.play();

    // lcd.printf(P("\f\n\n  %x"), midi.read_register(SCI_STATUS));
    // lcd.printf(P("\n  %x"), midi.read_register(SCI_VOL));
    // lcd.printf(P("\n  %u   "), x);
  }
}
