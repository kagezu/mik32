#include "core.h"
#include "menu.h"

typedef enum : uint8_t {
  // Normal,
  // ViewMode,
  Freq,
  VoltageScale,
  VoltageType,
  ZeroLevel,
} OSC_Mode;


const char *mode_text[] =
{
  "Normal",
  // "ViewMode",
  "ЧАСТОТА",
  "НАПРЯЖЕНИЕ",
  "AC / DC",
  "Zero Level",
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
/*
Discrete Dumpty = { 0, 0, {} };
Discrete Fq = { 36, 12, {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000} };
Discrete VScale = { 0, 6, {100, 50, 20, 10, 5, 2, 1} };
Discrete VType = { 0, 1, {'D', 'A'} };
Discrete ZLevel = { 0, (uint16_t)-1, {} };

Discrete *param[] = { &Dumpty, &Fq, &VScale, &VType, &ZLevel };
*/
//////////////////////////////////////////////////


int _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000 };
int _vsc[] = { 100, 50, 20, 10, 5, 2, 1 };

void t() {};


MItem Fq = { "ЧАСТОТА: ", OptionType, 9, 0, 13, _fq };
MItem VScale = { "НАПРЯЖЕНИЕ: ", OptionType, 0, 0, 7, _vsc };
MItem VType = { "AC/DC: ",  ValueType, 0, 0, 2 };
MItem ZLevel = { "Zero Level: ", ValueType, 0, -120, 120 };

int a = sizeof(Fq);

MItem menu = { "Menu: ", ListType, 0, 0, 4, { &Fq, &VScale, &VType, &ZLevel} };

