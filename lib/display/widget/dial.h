#pragma once
#include "widget.h"

class Dial : public Widget {
public:
  Dial() {}

  void init()
  {
    static uint16_t data[] = {
     GFX_round_rect, X, Y, 30, 30, 5
    };
    graph = data;
  }

  void draw()
  {}
protected:
};
