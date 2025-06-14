#pragma once
#include "core.h"

#define MENU_NAME_COUNT 20

///////////////////////// Item ////////////////////////////

class MItem {
protected:
  const char name[MENU_NAME_COUNT];
public:
  const char *get_name() const { return name; }
  virtual reg get_value() = 0;
  virtual  void *get_option() = 0;
  virtual  void next(int8_t) = 0;
  virtual reg select() = 0;
};

///////////////////////// Select //////////////////////////

template<const reg N>
class MSelect : public MItem {
protected:
  reg value;
public:
  reg get_value() { return value; }

  void next(int8_t step)
  {
    if (active) return list[value]->next(step);
    else value += step;
    if (value >= N) value = N - 1;
    if (value < 0) value = 0;
  }
};

///////////////////////// Option //////////////////////////

template<typename T, const reg N>
class MOption : public MSelect<N> {
protected:
  T option[N];

public:
  void *get_option() { return &option[value]; }
  constexpr reg select() const { return 1; }
};

////////////////////////// List ///////////////////////////

template<const reg N>
class MList : public MSelect<N> {
protected:
  bool active;
  MItem *list[N];

public:
  const char *get_name() const
  {
    if (active) return list[value]->get_name();
    return MItem::get_name();
  }

  const char get_value() const
  {
    if (active) return list[value]->get_value();
    return MSelect::get_value();
  }

  void next(int8_t step)
  {
    if (active) return list[value]->next(step);
    else value += step;
    if (value >= N) value = N - 1;
    if (value < 0) value = 0;
  }

  reg select()
  {
    reg level = 0;
    if (active) {
      level = list[value]->select();
      if (level-- > 0) active = false;
    }
    else active = true;
    return level;
  }
};

///////////////////////// Action //////////////////////////

class MAction : public MItem {
protected:
  const reg level;
  const void(*action)();
public:
  const reg select() const { action(); return level; }
};

///////////////////////////////////////////////////////////

// const struct M m[] = {
//   {
//  "ЧАСТОТА",36, 12,
//  {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000}
//   },
//   {
//  "НАПРЯЖЕНИЕ",0, 6,
//  {100, 50, 20, 10, 5, 2, 1}
//   }
