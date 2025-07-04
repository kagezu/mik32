#pragma once
#include "core.h"
#include "printf.h"

enum MenuType : int8_t {
  RootType,
  ListType,
  ActionType,
  ValueType,
  TextType,
  CharType,
  IntType
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
  template<typename T>
  T get_item() { return ((T *)(link[0]))[value]; }
  MItem *get(reg v) { return ((MItem *)(link[v])); }

  void get_path(PrintF *);
  void next(reg);
  reg select();


};
