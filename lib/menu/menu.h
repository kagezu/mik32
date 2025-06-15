#pragma once
#include "core.h"

#define MENU_NAME_COUNT 20

enum MenuType : reg {
  ItemType,
  ListType,
  ActionType,
  OptionType
};

#define PGET(type, prop)   (((type *)this)-> ## prop)

///////////////////

class MItem {
public:
  const reg type;
  const char name[MENU_NAME_COUNT];

  MItem *get()
  {
    if (type == ListType)
      if (PGET(MList, active))
        return PGET(MList, list)[PGET(MList, value)];
    return this;
  }

  MItem *get(reg _value)
  {
    if (type == ListType)
      if (PGET(MList, active))
        return PGET(MList, list)[_value];
    return this;
  }

  const char *get_name() const
  {
    if (type == ListType)
      if (PGET(MList, active))
        return PGET(MList, list)[PGET(MList, value)]->name;
    return name;
  }

  reg get_value()
  {
    switch (type) {
      case ListType:
        if (PGET(MList, active))
          return PGET(MList, list)[PGET(MList, value)]->get_value();

      case OptionType:
        return PGET(MOption, value);
    }
    return -1;
  }

  void next(reg step)
  {
    switch (type) {
      case ListType:
        if (PGET(MList, active))
          return PGET(MList, list)[PGET(MList, value)]->next(step);

      case OptionType:
        PGET(MValue, value) += step;
        if (PGET(MValue, value) > PGET(MValue, max)) PGET(MValue, value) = PGET(MValue, max);
        if (PGET(MValue, value) < 0) PGET(MValue, value) = 0;
    }
  }

  reg select()
  {
    reg level = 0;

    switch (type) {
      case ListType:
        if (PGET(MList, active))
          level = PGET(MList, list)[PGET(MList, value)]->select();
        else PGET(MList, active) = true;
        if (level-- > 0) PGET(MList, active) = false;
        break;

      case ActionType:
        PGET(MAction, action)();
        level = PGET(MAction, level);
        break;

      default:
        level = 1;
    }

    return level;
  }

};

///////////////////

class MValue : public MItem {
public:
  reg value;
  reg max;
};

///////////////////

class MList : public MValue {
public:
  bool active;
  MItem *list[];
};

///////////////////

class MOption : public MValue {
public:
  int list[];
};

///////////////////

class MAction : public MItem {
public:
  const reg level;
  const void(*action)();
};
