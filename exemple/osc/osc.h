#include "core.h"
#include "menu.h"

const char *_type[] = { "OSC", "FFT" };
const int _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000 };
const int _vsc[] = { 100, 50, 20, 10, 5, 2, 1 };
const char _vt[] = { 'A', 'D' };

MItem OType = { "ВЫВОД ", OptionType, 1, 0, 1, _type };
MItem Fq = { "ЧАСТОТА ", OptionType, 3, 0, 12, _fq };
MItem VScale = { "НАПРЯЖЕНИЕ ", OptionType, 0, 0, 6, _vsc };
MItem VType = { "AC/DC ",  OptionType, 1, 0, 1, _vt };
MItem ZLevel = { "Zero Level ", ValueType, 0, -120, 120, nullptr };

MItem menu = { "МЕНЮ: ", ListType, 0, 0, 4, { &OType ,&Fq, &VScale, &VType, &ZLevel} };

