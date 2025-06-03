#include "core.h"

typedef enum : uint8_t {
  // Normal,
  // ViewMode,
  Freq,
  VoltageScale,
  VoltageType,
  ZeroLevel,
  // Threshold,
  CountMode
} OSC_Mode;


const char mode_text[][24] =
{
  // "Normal",
  // "ViewMode",
  "ЧАСТОТА",
  "НАПРЯЖЕНИЕ",
  "AC / DC",
  "Zero Level",
  // "Threshold",
  "CountMode"
};

class Discrete {
public:
  int16_t counter;
  const uint16_t max;
  const uint16_t value[];

  uint16_t get_value()
  {
    if (counter > (max << 2)) counter = max << 2;
    if (counter < 0) counter = 0;
    return value[counter >> 2];
  }
};

Discrete Fq = { 36, 12, {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000} };
Discrete VScale = { 0, 6, {100, 50, 20, 10, 5, 2, 1} };
Discrete VType = { 0, 1, {'D', 'A'} };
Discrete ZLevel = { 0, (uint16_t)-1, {} };

Discrete *param[] = { &Fq, &VScale, &VType, &ZLevel };

