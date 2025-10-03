// #include "gpio.h"
#pragma once
#include <avr.h>

constexpr u8 GP_Float = 0x00;  // floating
constexpr u8 GP_VCC = 0x02;    // pull-up
constexpr u8 GP_GND = 0x00;    // pull-down
constexpr u8 GP_Open = 0x00;   // open-drain

// Для совместимости

constexpr u8 GP_IO = 0x00;     // general IO
constexpr u8 GP_Serial = 0x00; // 1я функция
constexpr u8 GP_Timer = 0x00;  // 2я функция
constexpr u8 GP_Analog = 0x00; // 3я функция

constexpr u8 GPO_2mA = 0x01;
constexpr u8 GPO_4mA = 0x01;
constexpr u8 GPO_8mA = 0x01;
constexpr u8 GPO_Max = 0x01;

constexpr u8 GPO_50MHz = 0x01;
constexpr u8 GPO_10MHz = 0x01;
constexpr u8 GPO_2MHz = 0x01;

typedef struct {
  volatile uint8_t PIN;
  volatile uint8_t DDR;
  volatile uint8_t PORT;
} GPIO_TypeDef;

constexpr uc8 PB = 0x0;
constexpr uc8 PC = 0x1;
constexpr uc8 PD = 0x2;

template <uc8 N, uc8 PINx>
class Pin {
private:
  constexpr static GPIO_TypeDef *GPIOx() { return  ((GPIO_TypeDef *)&PINB) + N; }

public:
  INLINE void set() { GPIOx()->PORT = 1 << PINx; }
  INLINE void clr() { GPIOx()->PORT &= ~(1 << PINx); }
  INLINE void inv() { GPIOx()->PORT ^= 1 << PINx; }
  INLINE void out(bool data) { if (data) set(); else clr(); }
  INLINE u8 get() { return GPIOx()->PIN & (1 << PINx); }
  INLINE void init(uc8 conf = GP_IO)
  {
    if (conf & GPO_Max) GPIOx()->DDR = 1 << PINx;
    else GPIOx()->DDR &= ~(1 << PINx);
    if (conf & GP_VCC) GPIOx()->PORT = 1 << PINx;
    else GPIOx()->PORT &= ~(1 << PINx);
  }
};

template <uc32 N, uc32 PINS>
class Port {
private:
  constexpr static GPIO_TypeDef *GPIOx() { return  ((GPIO_TypeDef *)&PINB) + N; }

public:
  INLINE void set(u8 data) { GPIOx()->PORT |= data; }
  INLINE void clr(u8 data) { GPIOx()->PORT &= ~data; }
  INLINE void inv(u8 data) { GPIOx()->PORT ^= data; }
  INLINE void out(u8 data) { GPIOx()->PORT = data; }
  INLINE u8 get() { return GPIOx()->PIN; }
  void init(uc32 conf = GP_IO)
  {
    if (conf & GPO_Max) GPIOx()->DDR |= PINS;
    else GPIOx()->DDR &= ~PINS;
    if (conf & GP_VCC) GPIOx()->PORT |= PINS;
    else GPIOx()->PORT &= ~PINS;
  }
};
