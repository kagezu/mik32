#include "menu.h"

#define MLIST(value)  ((MItem *)(link[value]))

void MItem::next(reg step)
{
  switch (type) {
    case ListType:
      if (active) return MLIST(value)->next(step);

    default:
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

      // case ActionType:
      //   ((void (*)())(link[0]))();
      //   current_level = level;
      //   break;

    default:
      current_level = 1;
  }

  return current_level;
}

void MItem::print(PrintF *out)
{
  switch (type) {
    case ListType:
      out->printf("%s ", name);
      if (active) MLIST(value)->print(out);
      else out->printf("\1%s\1 ", MLIST(value)->name);
      break;

    case TextType:
      out->printf("%s ", name);
      out->printf("\1%s\1 ", get_item<char *>());
      return;

    case CharType:
      out->printf("%s ", name);
      out->printf("\1%c\1 ", get_item<char>());
      return;

    case IntType:
      out->printf("%s ", name);
      out->printf("\1%u\1 ", get_item<int>());
      return;

    case ValueType:
      out->printf("%s ", name);
      out->printf("\1%u\1 ", value);
  }
}
