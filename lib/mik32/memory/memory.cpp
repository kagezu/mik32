#include "memory.h"

uint16_t Memory::_var;
MemoryBlock *Memory::_ptr;

void Memory::init(uint16_t start, uint16_t length)
{
  _stack.init(start + length);
  _stack.push()->init(start, length - sizeof(MemoryBlock));
  _status = MEM_FREE;
}

uint16_t Memory::heap()
{
  I_SAVE;
  _var = 0;
  _stack.each(this->_sum);
  I_REST;
  return _var;
}

void Memory::malloc(void **var, uint16_t size)
{
  I_SAVE;
  MemoryBlock *block = _find(size);
  if (block) block->use((uint16_t)var); // Передаём ссылку на указатель в запись блока
  else  *var = nullptr; // Обнуляем указатель
  I_REST;
}

uint8_t *Memory::malloc(uint16_t size)
{
  I_SAVE;
  MemoryBlock *block = _find(size);
  uint8_t *ptr;
  if (block) { block->use(); ptr = (uint8_t *)block->start(); } // Указать, что блок используется и заблокирован
  else ptr = nullptr;
  I_REST;
  return ptr;
}

void Memory::free(void **var)
{
  if ((uint16_t)var >= RAMSTART) { // Указатель находиться в допустимой области памяти
    I_SAVE;
    _var = (uint16_t)var;
    uint8_t index = _stack.findindex(this->_find_link);
    if (index && _stack.at(index)->is_used()) {
      _stack.at(index)->free();
      _union(index); // Объединить пустые блоки, если они рядом
    }
    I_REST;
  }
  *var = nullptr; // Обнуляем указатель
}

void Memory::free(void *var)
{
  if ((uint16_t)var >= RAMSTART) { // Ссылка указывает на допустимую область памяти
    I_SAVE;
    _var = (uint16_t)var;
    uint8_t index = _stack.findindex(this->_find_start);
    if (index && _stack.at(index)->is_used()) {
      _stack.at(index)->free();
      _union(index); // Объединить пустые блоки, если они рядом
    }
    I_REST;
  }
}

void Memory::shrink()
{
  uint8_t length = _stack.size() - 1;
  uint8_t i = 1;
  while (i < length) {
    MemoryBlock *block = _stack.at(i++);
    if (block->is_fixed() || block->is_used()) continue;
    if (_stack.at(i)->is_fixed()) continue;
    uint16_t start = block->start();
    uint16_t size = block->size();
    memmove((void *)start, (void *)_stack.at(i)->start(), _stack.at(i)->size());
    block->size(_stack.at(i)->size());
    _stack.at(i)->size(size);
    _stack.at(i)->start(start + block->size());
    block->use(_stack.at(i)->link());
    _stack.at(i)->free();
    _union(i);
    length = _stack.size() - 1;
  }
}

MemoryBlock *Memory::_find(uint16_t size)
{
  _var = size;
  _ptr = nullptr;
  MemoryBlock *top = _stack.pop(); // Исключаем из поиска блок на вершине стека
  _stack.each(this->_near); // Поиск блока наиболее близкого размера
  _stack.push(); // Восстанавливаем блок на вершине стека
  if (_ptr == nullptr) {// Блока для выделения памяти не найдено
    if (top->size() > size + MEM_BLOCK_MIN_SIZE) // Проверяем блок на вершине стека
      _ptr = top;
    else {
      if (heap() >= size + MEM_BLOCK_MIN_SIZE) { // Есть ли смысл в дефрагментации
        // while (_status); // Здесь должно быть ожидание разблокировки памяти, через передачу управления другим задачам
        shrink(); // Сжатие памяти
        _stack.each(this->_near); // Повторный поиск блока памяти, включая на вершине стека
        // Последний блок должен быть пустым, так как из него выделяется память в стек
        if (_ptr == top && top->size() < size + MEM_BLOCK_MIN_SIZE) _ptr = nullptr;
      }
    }
  }
  if (_ptr != nullptr) { // Подходящий блок памяти найден
    uint16_t heap = _ptr->size(); // Свободная память выделяемого блока
    uint16_t start = _ptr->start(); // Начало  выделяемого блока
    // Дробим блок, если новый блок будет больше минимального и есть память на стек
    if (heap >= size + MEM_BLOCK_MIN_SIZE && top->size() > sizeof(MemoryBlock)) {
      _ptr->size(size); // Новый размер блока
      _stack.insert_post(_ptr)->init(start + size, heap - size); // Новый блок
      _stack.head()->size(_stack.head()->size() - sizeof(MemoryBlock)); // Уменьшаем память блока на вершине стека
    }
  }
  return _ptr;
}

void Memory::_union(uint8_t index)
{
  uint8_t i = index - 1;
  MemoryBlock *ptr = _stack.at(i);
  if (ptr && !ptr->is_used()) {
    ptr->size(ptr->size() + _stack.at(index)->size());
    _stack.erase(index--);
    _stack.head()->size(_stack.head()->size() + sizeof(MemoryBlock)); // Возвращаем память стека
  }
  i = index + 1;
  ptr = _stack.at(i);
  if (ptr && !ptr->is_used()) {
    _stack.at(index)->size(ptr->size() + _stack.at(index)->size());
    _stack.erase(i);
    _stack.head()->size(_stack.head()->size() + sizeof(MemoryBlock)); // Возвращаем память стека
  }
}
