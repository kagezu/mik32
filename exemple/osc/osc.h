#include "core.h"
#include "menu.h"

enum AppModeEnum :int {
  MODE_OSC,
  MODE_FFT,
  MODE_SPEC
};

const int _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };
const int _vsc[] = { 200, 100, 50, 20, 10, 5, 2, 1 };
const char *_vt[] = { "AC", "DC" };
const char *_ft[] = { "Off", "Sum", "Log" };

ADD_MINT(FqScale, "us x", 1, _fq);
ADD_MINT(VScale, "mV x", 1, _vsc);
ADD_MTEXT(VType, "Current", 0, _vt);
ADD_MVALUE(ZeroLevel, "Setup zero", 0, -200, 200);
ADD_MTEXT(FType, "Filter", 0, _ft);
ADD_MITEM(MExit, "Return");


MItem OSC_Menu = { "OSC", ListType, 0, 0, 0, 4 , { &MExit, &FqScale, &VScale, &VType, &ZeroLevel } };
MItem FFT_Menu = { "FFT", ListType, 0, 0, 0, 3 , { &MExit, &FqScale, &VScale, &FType } };
MItem SPEC_Menu = { "SPEC", ListType, 0, 0, 0, 2 , { &MExit, &FqScale, &VScale } };

MItem menu = { "", ListType, 0, 0, 0, 2, { &OSC_Menu, &FFT_Menu, &SPEC_Menu } };
