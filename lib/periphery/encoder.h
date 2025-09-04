// #include "encoder.h"
#include "core.h"
#include "pins.h"

class Encoder {
public:
  int count = 2, c1 = 0, c2 = 0;
  bool a0, b0;

public:
  Encoder()
  {
    ENCODER_A(GPIO);
    ENCODER_B(GPIO);
    ENCODER_SW(GPIO);
    ENCODER_A(IN);
    ENCODER_B(IN);
    ENCODER_SW(IN);
    ENCODER_A(P_VCC);
    ENCODER_B(P_VCC);
    ENCODER_SW(P_VCC);

  #ifdef ENCODER_GND
    ENCODER_GND(GPIO);
    ENCODER_GND(OUT);
    ENCODER_GND(CLR);
    ENCODER_SW(P_GND);
  #endif
  }

  bool is_push()
  {
  #ifdef ENCODER_GND
    return ENCODER_SW(GET);
  #else
    // return !ENCODER_SW(GET);
  #endif
  }

  int scan()
  {
    int result;

    // состояние контактов A и B ?
    bool a = (bool)ENCODER_A(GET);
    bool b = (bool)ENCODER_B(GET);

    // если состояние контакта A изменилось и оно такое как B
    // то попорот по часовой, иначе против
    if (a != a0) count += a ^ b ? -1 : 1;

    // если состояние контакта B изменилось и и оно такое как A
    // то попорот против часовой, иначе по часовой
    if (b != b0) count += a ^ b ? 1 : -1;

    // запоминаем состояние контактов
    a0 = a;
    b0 = b;

    // один "щелчок" энкодера соответствует 4-рём позициям
    c1 = count >> 2;
    // если произошёл сдвиг на 4 позиции, фиксируем поворот
    result = c1 - c2;
    c2 = c1;
    return result;
  }
};
