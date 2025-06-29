#pragma once
#include "core.h"

enum MenuType : int8_t {
  ItemType,
  ListType,
  ActionType,
  OptionType,
  ValueType
};


class MItem {
public:
  const char *name;
  const int8_t type;
  int8_t value;
  union {
    int8_t min;
    int8_t active;
    int8_t level;
  };
  int8_t max;
  const void *link[];

public:
  MItem *get(reg);
  char *get_path(bool f = true);
  void next(reg);
  reg select();

  template<typename T>
  T get_item()
  {
    if (type == OptionType)
      return ((T *)(link[0]))[value];
    return 0;
  }

protected:
  static char *ptr;
  static char buffer[];
};
