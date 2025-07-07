#pragma once
#include "core.h"
#include "printf.h"

#define ADD_MTEXT(name, text, start, arr) MItem name = { text, TextType, start, 0, sizeof(arr) / sizeof(arr[0]) - 1, arr }
#define ADD_MINT(name, text, start, arr) MItem name = { text, IntType, start, 0, sizeof(arr) / sizeof(arr[0]) - 1, arr }
#define ADD_MCHAR(name, text, start, arr) MItem name = { text, CharType, start, 0, sizeof(arr) / sizeof(arr[0]) - 1, arr }
#define ADD_MVALUE(name, text, start, min, max) MItem name = { text, ValueType, start, min, max, nullptr }
// #define ADD_MACTIVE(name, text, start, arr) MItem name = { text, ActiveType, start, 0, sizeof(arr) / sizeof(arr[0]) , arr }

enum MenuType : int {
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
  const int type;
  int value;
  union {
    int min;
    int active;
    int level;
  };
  int max;
  const void *link[];

public:
  template<typename T>
  T get_item() { return ((T *)(link[0]))[value]; }
  MItem *get(reg v) { return ((MItem *)(link[v])); }

  void print(PrintF *);
  void next(reg);
  reg select();


};
