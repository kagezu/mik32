#pragma once
#include "core.h"
#include "printf.h"

#define ADD_MTEXT(name, text, start, arr) MItem name = { text, TextType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr }
#define ADD_MINT(name, text, start, arr) MItem name = { text, IntType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr }
#define ADD_MCHAR(name, text, start, arr) MItem name = { text, CharType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr }
#define ADD_MVALUE(name, text, start, min, max) MItem name = { text, ValueType, start, min, max, nullptr }
#define ADD_MROOT(name, text, start, arr) MItem name = { text, RootType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr}
// #define ADD_MLIST(name, text, start, arr) MItem name = { text, ListType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr }
// #define ADD_MACTIVE(name, text, start, arr) MItem name = { text, ActiveType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr }

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
