#pragma once
#include "core.h"
#include "printf.h"

#define ADD_MTEXT(name, text, value, arr) MItem name = { text, TextType, 0, value, 0, sizeof(arr) / sizeof(arr[0]) - 1, arr }
#define ADD_MINT(name, text, value, arr) MItem name = { text, IntType, 0, value, 0, sizeof(arr) / sizeof(arr[0]) - 1, arr }
#define ADD_MCHAR(name, text, value, arr) MItem name = { text, CharType, 0, value, 0, sizeof(arr) / sizeof(arr[0]) - 1, arr }
#define ADD_MVALUE(name, text, value, min, max) MItem name = { text, ValueType, 0, value, min, max, nullptr }
#define ADD_MITEM(name, text) MItem name = { text, ItemType }
// #define ADD_MACTIVE(name, text, value, arr) MItem name = { text, ActiveType, value, 0, sizeof(arr) / sizeof(arr[0]) , arr }

enum MenuType : char {
  ListType,
  ActionType,
  ValueType,
  TextType,
  CharType,
  IntType,
  ItemType
};

class MItem {
public:
  const char *name;
  const char type;
  char pass;
  int value;
  int min;
  int max;
  const void *link[];

public:
  template<typename T>
  T get_item() { return ((T *)(link[0]))[value]; }
  MItem *get(reg v) { return ((MItem *)(link[v])); }

  void print(PrintF *);
  void next(reg);
  char select();
  int *save(int *);
  int *load(int *);
};
