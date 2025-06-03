#include "core.h"

typedef enum : uint8_t {
  // Normal,
  // ViewMode,
  Freq,
  VoltageScale,
  // VoltagDiv,
  // ZeroLevel,
  // Threshold,
  CountMode
} OSC_Mode;


const char mode_text[][14] =
{
  // "Normal",
  // "ViewMode",
  "Freq",
  "Voltage Scale",
  // "VoltagDiv",
  // "ZeroLevel",
  // "Threshold",
  "CountMode"
};

class Discrete {
public:
  int8_t counter;
  uint16_t max;
  uint16_t value[10];

  uint16_t get_value()
  {
    if (counter > max) counter = max;
    if (counter < 0) counter = 0;
    return value[counter >> 2];
  }
};

Discrete param[] = {
  {12, 6 * 4, {1000, 500, 200, 100, 50, 20, 10, 5, 2, 1} },
  {0, 6 * 4, {100, 50, 20, 10, 5, 2, 1} }
};

