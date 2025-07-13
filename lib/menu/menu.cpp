#include "menu.h"

#define MLIST(value)  ((MItem *)(link[value]))

void MItem::next(reg step)
{
  switch (type) {
    case ListType:
      if (pass) return MLIST(value)->next(step);

    default:
      value += step;
      if (value > max) value = max;
      if (value < min) value = min;
  }
}

char MItem::select()
{
  char level = 0;

  switch (type) {
    case ListType: {
        if (pass) level = MLIST(value)->select();
        else switch (MLIST(value)->type) {
          case ActionType:
            ((void (*)())(link[0]))();
          case ItemType:
            level = 2;
            break;

          default:
            pass = true;
            break;
        }
        if (level-- > 0) pass = false;
        break;
      }

    default:
      level = 1;
  }

  return level;
}

void MItem::print(PrintF *out)
{
  switch (type) {
    case ListType:
      out->printf("%s ", name);
      if (pass) MLIST(value)->print(out);
      else out->printf("\1%s\1 ", MLIST(value)->name);
      break;

    case TextType:
      out->printf("%s ", name);
      out->printf("\1%s\1 ", get_item<char *>());
      break;

    case CharType:
      out->printf("%s ", name);
      out->printf("\1%c\1 ", get_item<char>());
      break;

    case IntType:
      out->printf("%s ", name);
      out->printf("\1%u\1 ", get_item<int>());
      break;

    case ValueType:
      out->printf("%s ", name);
      out->printf("\1%u\1 ", value);
      break;
  }
}

int *MItem::save(int *ptr)
{
  switch (type) {
    case ListType:
      *ptr++ = value;
      for (int i = 0; i <= max; i++) {
        ptr = MLIST(i)->save(ptr);
      }
      break;

    case TextType:
    case CharType:
    case IntType:
    case ValueType:
      *ptr++ = value;
  }
  return ptr;
}

int *MItem::load(int *ptr)
{
  switch (type) {
    case ListType:
      value = *ptr++;
      for (int i = 0; i <= max; i++) {
        ptr = MLIST(i)->load(ptr);
      }
      break;

    case TextType:
    case CharType:
    case IntType:
    case ValueType:
      value = *ptr++;
  }
  return ptr;
}
