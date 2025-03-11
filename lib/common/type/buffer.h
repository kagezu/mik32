// #include "type/buffer.h"
#pragma once
#include <core.h>

/*
#include <type/buffer.h>
#### Buffer<T>
+ read() <-- -- [ tail ] ... [ head ] <-- write()
+ shift() <-- - [ tail ] ... [ head ] <--  push()
+ unshift() --> [ tail ] ... [ head ] -->   pop()
+ tail()  =  buffer [ tail ]
+ head()  =  buffer [ head ]
+ length() clear()
*/
template <typename T>
class Buffer {
protected:
  T *_buffer;
  reg _head;    // Голова
  reg _tail;    // Хвост
  reg _size;    // Максимальный размер
  reg _heap;    // Размер кучи

public:
  // Конструктор
  Buffer(T *buffer, reg size)
  {
    _buffer = buffer;
    _size = size;
    clear();
  }

  // Очищает буфер
  void clear()
  {
    _heap = _size;
    _head = _tail = 0;
  }

  // Текущий размер буфера
  reg length()
  {
    return _size - _heap;
  }

  // Свободный размер буфера
  reg heap()
  {
    return _heap;
  }

  // Возвращает элемент с головы, без удаления, даже если буфер пуст

  T head()
  {
    return _buffer[_head ? _head - 1 : _size - 1];
  }

  // Возвращает элемент с хвоста, без удаления, даже если буфер пуст

  T tail()
  {
    return _buffer[_tail];
  }

  // Возвращает элемент с хвоста либо пустой элемент, если буфер пуст

  T shift()
  {
    return read();
  }

  // Записывает элемент в буфер, если буфер не полон
  void write(T data)
  {
    if (!_heap) return;                       // Буфер полон
    _buffer[_head++] = data;
    _head = _head == _size ? 0 : _head;
    _heap--;
  }

  // Возвращает элемент из буфера либо пустой элемент, если буфер пуст
  T read()
  {
    if (_heap == _size) return T();          // Буфер пуст
    reg index = _tail++;
    _tail = _tail == _size ? 0 : _tail;
    _heap++;
    return _buffer[index];
  }

  // Записывает элементы в буфер до заполнения буфера, то что не влезло - обрезается
  void write(T *data, reg length)
  {
    T *target = _buffer + _head;
    reg count = _size - _head;                  // Линейный размер пространства
    length = length > _heap ? _heap : length; // Размер перемещаемых данных ограничен кучей
    _heap -= length;                          // Уменьшаем кучу
    _head += length;                          // Двигаем голову
    if (length >= count) {                    // Если будет достигнут конец буферного пространства
      length -= count;                        // Остаток на запись
      _head = length;                         // Новое положение головы
      while (count--)*target++ = *data++;
      target = _buffer;                       // продолжим запись с начала буфера
    }
    while (length--)*target++ = *data++;
  }

  // Возвращает элементы из буфера до опустошения
  void read(T *data, reg length)
  {
    T *source = _buffer + _tail;
    reg count = _size - _tail;                  // Линейный размер пространства
    length = length > _size - _heap
      ? _size - _heap : length;               // Размер перемещаемых данных ограничен их количеством
    _heap += length;                          // высвобождаем кучу
    _tail += length;                          // Двигаем хвост
    if (length >= count) {                    // Если будет достигнут конец буферного пространства
      length -= count;                        // Остаток на считывание
      _tail = length;                         // Новое положение хвоста
      while (count--) *data++ = *source++;
      source = _buffer;                       // Продолжим читать с начала буфера
    }
    while (length--) *data++ = *source++;
  }

  // Добавляет элемент с головы, возвращает его индекс
  reg push(T data)
  {
    if (!_heap) return -1;                 // Буфер полон
    reg index = _head;
    _buffer[_head++] = data;
    _head = _head == _size ? reg(0) : _head;
    _heap--;
    return index;
  }

  // Извлекает элемент с головы
  T pop()
  {
    if (_heap == _size) return 0;          // Буфер пуст
    if (_head == 0) _head = _size - 1;
    _heap++;
    return _buffer[_head];
  }

  // Добавляет элемент в хвост, возвращает его индекс
  reg unshift(T data)
  {
    if (!_heap) return -1;                 // Буфер полон
    if (_tail == 0) _tail = _size;
    _buffer[--_tail] = data;
    _heap--;
    return _tail;
  }
};
