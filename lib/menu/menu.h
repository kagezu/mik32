#pragma once
#include "core.h"

enum MenuType : int8_t {
  ItemType,
  ListType,
  ActionType,
  OptionType,
  ValueType
};

#define MLIST(value)  ((MItem *)(link[value]))

constexpr int MBUFFER_SIZE = 64;

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
  void *link[];

public:
  MItem *get()
  {
    if (type == ListType)
      if (active) return MLIST(value)->get();
    return this;
  }

  MItem *get(reg _value)
  {
    if (type == ListType)
      if (active) return MLIST(_value);
    return this;
  }

  const char *get_name() const
  {
    if (type == ListType)
      if (active) return MLIST(value)->name;
    return name;
  }

  char *get_path() const
  {
    const char *str = name;
    while (*ptr) *ptr++ = *str++;
    if (type == ListType)
      if (active) return MLIST(value)->get_path();
    *ptr = '\0';
    ptr = buffer;
    return buffer;
  }

  int8_t get_value()
  {
    switch (type) {
      case ListType:
        if (active) return MLIST(value)->get_value();
      case OptionType: return value;
    }
    return 0;
  }

  template<typename T>
  T get_item()
  {
    switch (type) {
      case OptionType: return ((T *)(link[0]))[value];
    }
    return 0;
  }

  void next(reg step)
  {
    switch (type) {
      case ListType:
        if (active) return MLIST(value)->next(step);
      case OptionType:
      case ValueType:
        value += step;
        if (value >= max) value = max;
        if (value < min) value = min;
    }
  }

  reg select()
  {
    reg current_level = 0;

    switch (type) {
      case ListType:
        if (active)
          current_level = MLIST(value)->select();
        else active = true;
        if (current_level-- > 0) active = false;
        break;

      case ActionType:
        ((void (*)())(link[0]))();
        current_level = level;
        break;

      default:
        current_level = 1;
    }

    return current_level;
  }

protected:
  static char *ptr;
  static char buffer[MBUFFER_SIZE];
};
