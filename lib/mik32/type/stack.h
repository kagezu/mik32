#pragma once
#include "int.h"

/*
#### Stack<T, u32> T - element, u32 - index
+ clear() / size()
+ push(T) --> [ sp ]
+ *pop () <-- [++sp]
+ *head() <== [sp+1]
+ *top () <== [ sp ]
+ each() / find() / findindex()
+ erase(u32) / insert_next(T*) / at(u32)
*/
template <typename T>
class Stack {
protected:
  T *_stack = nullptr;
  u32 _size = 0;

public:
  Stack() {}
  Stack(u32 sp) : _stack((T *)(sp - sizeof(T))) {}

  INLINE void init(u32 sp) { _stack = (T *)(sp - sizeof(T)); _size = 0; }
  INLINE void clear() { _stack -= _size; _size = 0; }
  INLINE u32 size() { return _size; }
  INLINE void push(T data) { *_stack-- = data; _size++; }
  INLINE T *push() { T *ptr = _stack--; _size++; return ptr; }
  INLINE T *pop() { _size--; return ++_stack; }
  INLINE T *top() { return _stack; }
  INLINE T *head() { return _stack + 1; }

public:
  void each(void callback(T *ptr));
  u32 findindex(bool callback(T *ptr));
  T *find(bool callback(T *ptr));
  void erase(u32 index);
  T *insert_post(T *ptr);
  T *at(u32 index);
};

template <typename T>
void Stack<T>::each(void callback(T *ptr))
{
  u32 index = _size;
  T *ptr = _stack;
  while (index--) callback(++ptr);
}

template <typename T>
u32 Stack<T>::findindex(bool callback(T *ptr))
{
  u32 index = _size;
  T *ptr = _stack;
  while (index) {
    if (callback(++ptr)) break;
    index--;
  }
  return index;
}

template <typename T>
T *Stack<T>::find(bool callback(T *ptr))
{
  u32 index = _size;
  T *ptr = _stack;
  while (index) {
    if (callback(++ptr)) break;
    index--;
  }
  return index ? ptr : nullptr;
}

template <typename T>
void Stack<T>::erase(u32 index)
{
  if (index > _size || index == 0) return;
  T *ptr = _stack + _size - index + 1;
  index -= _size;
  while (index++) {
    *ptr = *(ptr - 1);
    ptr--;
  }
  _stack++;
  _size--;
}

template <typename T>
T *Stack<T>::insert_post(T *ptr)
{
  T *new_element = ptr - 1;
  ptr = _stack;
  while (ptr != new_element) {
    *ptr = *(ptr + 1);
    ptr++;
  }
  _stack--;
  _size++;
  return new_element;
}

template <typename T>
T *Stack<T>::at(u32 index)
{
  if (index > _size || index == 0) return nullptr;
  return _stack + _size - index + 1;
}
