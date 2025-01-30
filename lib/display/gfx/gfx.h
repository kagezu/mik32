#pragma once
#include "type/include.h"

class  GFX :private IDisplay {
protected:
  int16_t _fat = 2;

public:
  void fat(int16_t f) { _fat = f; }

  // Реализуется в данном классе
public:
  // Линии и фигуры
  void w_line(int16_t, int16_t, int16_t);
  void h_line(int16_t, int16_t, int16_t);
  void w_line_fat(int16_t, int16_t, int16_t);
  void h_line_fat(int16_t, int16_t, int16_t);

  void line(int16_t, int16_t, int16_t, int16_t);
  void round_rect(int16_t, int16_t, int16_t, int16_t, int16_t);
  void rect_line(int16_t, int16_t, int16_t, int16_t);
  void circle(int16_t, int16_t, int16_t);

  // Утолщённые линии и фигуры
  void line_fat(int16_t, int16_t, int16_t, int16_t);
  void rect_fat(int16_t, int16_t, int16_t, int16_t);
  void circle_fat(int16_t, int16_t, int16_t);

  // Полные фигуры
  void circle_fill(int16_t, int16_t, int16_t);
  void round_rect_fill(int16_t, int16_t, int16_t, int16_t, int16_t);

  // Реализуется в дочернем классе
  // public:
  //   virtual void pixel(int16_t, int16_t) = 0;
  //   virtual void rect(int16_t, int16_t, int16_t, int16_t) = 0;
};
