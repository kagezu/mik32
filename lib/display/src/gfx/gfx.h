#pragma once
#include "type/include.h"

class  GFX :private IDisplay {
protected:
  uint16_t _fat = 2;

public:
  void fat(uint16_t f) { _fat = f; }

  // Реализуется в данном классе
public:
  // Линии и фигуры
  void w_line(uint16_t, uint16_t, uint16_t);
  void h_line(uint16_t, uint16_t, uint16_t);
  void w_line_fat(uint16_t, uint16_t, uint16_t);
  void h_line_fat(uint16_t, uint16_t, uint16_t);

  void line(uint16_t, uint16_t, uint16_t, uint16_t);
  void round_rect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
  void rect_line(uint16_t, uint16_t, uint16_t, uint16_t);
  void circle(uint16_t, uint16_t, uint16_t);

  // Утолщённые линии и фигуры
  void line_fat(uint16_t, uint16_t, uint16_t, uint16_t);
  void rect_fat(uint16_t, uint16_t, uint16_t, uint16_t);
  void circle_fat(uint16_t, uint16_t, uint16_t);

  // Полные фигуры
  void circle_fill(uint16_t, uint16_t, uint16_t);
  void round_rect_fill(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
};
