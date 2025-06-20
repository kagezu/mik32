#pragma once
#include "type/display.h"

// для mik32 должно быть чётным + 1 байт
#define PRINT_BUFFER_SIZE   33

class PrintF :public IDisplay {
public:
  void printf(const char *, ...);
  void print(const char *);
  void print(char *, reg algin = 0);
  // char *print(int64_t);
  char *print(int32_t);
  char *print(int16_t);
  // char *print(uint64_t);
  char *print(uint32_t);
  char *print(uint16_t);
  char *print_h(uint64_t);
  char *print_h(uint32_t);
  char *print_h(uint16_t);
  uint16_t print_h(uint8_t);
  void print(char);

private:
  char buffer[PRINT_BUFFER_SIZE];

private:
  static reg strlen(char *);
};
