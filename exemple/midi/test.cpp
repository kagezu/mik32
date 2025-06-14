#include "config.h"
#include "ernani.mid.h"
#include "midi-play/midi-play.h"

LCD lcd;
VS1053 midi;
SPI spi;

int main(void)
{
  spi.init();
  lcd.init();
  midi.init();
  lcd.background(RGB(0, 16, 32));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);

  // midi.pgm_change(30);

  lcd.printf(P("\f  Started"));

  MIDI_Play play(&midi);

  play.open(ernani);

  while (true) {

    play.play();
    for (uint16_t i = 0; i < 500; i++)
      play.tick();

    // lcd.printf(P("\f\n\n  %x"), midi.read_register(SCI_STATUS));
    // lcd.printf(P("\n  %x"), midi.read_register(SCI_VOL));
    // lcd.printf(P("\n  %u   "), x);
  }
}
