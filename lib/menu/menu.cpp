#include "menu.h"

#define MLIST(value)  ((MItem *)(link[value]))

constexpr int MBUFFER_SIZE = 64;

char *MItem::ptr;
char  MItem::buffer[MBUFFER_SIZE];

MItem *MItem::get(reg _value)
{
  return MLIST(_value);
}

char *MItem::get_path(bool f)
{
  if (f) ptr = buffer;
  ptr = sjoin(ptr, name);
  if (type == ListType) {
    if (active) return MLIST(value)->get_path(false);
    ptr = sjoin(ptr, MLIST(value)->name);
  }
  if (type != ListType) sjoin(ptr, "*");
  return buffer;
}

void MItem::next(reg step)
{
  switch (type) {
    case ListType:
      if (active) return MLIST(value)->next(step);
      value += step;
      if (value > max) value = min;
      if (value < min) value = max;
      break;

    case ValueType:
    case OptionType:
      value += step;
      if (value > max) value = max;
      if (value < min) value = min;
  }
}

reg MItem::select()
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
