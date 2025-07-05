#include "core.h"
#include "menu.h"

const char *_type[] = { "OSC", "FFT", "SPEC" };
const int _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000 };
const int _vsc[] = { 100, 50, 20, 10, 5, 2, 1 };
const char _vt[] = { 'A', 'D' };

ADD_MTEXT(OType, "ВЫВОД", 0, _type);
ADD_MINT(Fq, "ЧАСТОТА", 3, _fq);
ADD_MINT(VScale, "НАПРЯЖЕНИЕ", 0, _vsc);
ADD_MCHAR(VType, "AC/DC", 1, _vt);
ADD_MVALUE(ZLevel, "Zero Level", 0, -120, 120);

MItem *_root[] = { &OType ,&Fq, &VScale, &VType, &ZLevel };

ADD_MROOT(menu, "МЕНЮ:", 0, _root);
