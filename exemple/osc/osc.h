#include "core.h"
#include "menu.h"

typedef enum : uint8_t {
  Freq,
  VoltageScale,
  VoltageType,
  ZeroLevel,
} OSC_Mode;


const int _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000 };
const int _vsc[] = { 100, 50, 20, 10, 5, 2, 1 };
const char _vt[] = { 'A', 'D' };

MItem Fq = { "ЧАСТОТА ", OptionType, 9, 0, 12, (int *)_fq };
MItem VScale = { "НАПРЯЖЕНИЕ ", OptionType, 0, 0, 6, (int *)_vsc };
MItem VType = { "AC/DC ",  OptionType, 1, 0, 1, (char *)_vt };
MItem ZLevel = { "Zero Level ", ValueType, 0, -120, 120, nullptr };

MItem menu = { "МЕНЮ: ", ListType, 0, 0, 3, { &Fq, &VScale, &VType, &ZLevel} };

