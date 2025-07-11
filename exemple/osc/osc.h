#include "core.h"
#include "menu.h"

enum AppModeEnum :int {
  MODE_OSC,
  MODE_FFT,
  MODE_SPEC
};

const char *_type[] = { "OSC", "FFT", "SPEC" };
const int32_t _fq[] = { 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };
const int _vsc[] = { 200, 100, 50, 20, 10, 5, 2, 1 };
const char *_vt[] = { "AC", "DC" };
const char *_ft[] = { "НЕТ", "СУММИРУЮЩИЙ", "ЛОГАРИФМ" };

ADD_MTEXT(AppMode, "РЕЖИМ", 0, _type);
ADD_MINT(FqScale, "МИК СЕК / ДЕЛ", 1, _fq);
ADD_MINT(VScale, "МИЛ ВОЛЬТ / ДЕЛ", 1, _vsc);
ADD_MTEXT(VType, "ОТН / АБС.", 0, _vt);
ADD_MVALUE(ZeroLevel, "УСТАНОВКА НУЛЯ", 0, -200, 200);
ADD_MTEXT(FType, "FFT ФИЛЬТР", 0, _ft);

MItem menu = { "МЕНЮ", ListType, 0, 0, 5 , { &AppMode ,&FqScale, &VScale, &VType, &FType, &ZeroLevel } };
