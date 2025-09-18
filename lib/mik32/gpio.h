#pragma once
#include "sdk/mik32_memory_map.h"
#include "sdk/pad_config.h"
#include "sdk/gpio.h"
#include "type/int.h"
#include "attribute.h"


#define GPIO(port, pin)     PAD_CONFIG->PORT_ ## port ## _CFG &= ~(0b11 << ( pin << 1))
#define SERIAL(port, pin)   PAD_CONFIG->PORT_ ## port ## _CFG |= (0b01 << ( pin << 1))
#define TIMER(port, pin)    PAD_CONFIG->PORT_ ## port ## _CFG |= (0b10 << ( pin << 1))
#define ANALOG(port, pin)   PAD_CONFIG->PORT_ ## port ## _CFG |= (0b11 << ( pin << 1))

#define DS_2MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b00 << ( pin << 1))
#define DS_4MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b01 << ( pin << 1))
#define DS_8MA(port, pin)   PAD_CONFIG->PORT_ ## port ## _DS |= (0b10 << ( pin << 1))

#define P_NC(port, pin)     PAD_CONFIG->PORT_ ## port ## _PUPD &= ~(0b11 << ( pin << 1))
#define P_VCC(port, pin)    PAD_CONFIG->PORT_ ## port ## _PUPD |= (0b01 << ( pin << 1))
#define P_GND(port, pin)    PAD_CONFIG->PORT_ ## port ## _PUPD |= (0b10 << ( pin << 1))

#define IN(port, pin)       GPIO_ ## port ->DIRECTION_IN = (1 << pin)
#define OUT(port, pin)      GPIO_ ## port ->DIRECTION_OUT = (1 << pin)

#define SET(port, pin)      GPIO_ ## port ->SET = (1 << pin)
#define CLR(port, pin)      GPIO_ ## port ->CLEAR = (1 << pin)
#define INV(port, pin)      GPIO_ ## port ->OUTPUT ^= (1 << pin)
#define GET(port, pin)      (GPIO_ ## port ->STATE & (1 << pin))

#define MASK(port, pin)     (1 << pin)
#define OUTPUT(port, pin)   (GPIO_ ## port ->OUTPUT)
#define STATE(port, pin)    (GPIO_ ## port ->STATE)


constexpr u32 GPIO_Port = 0x00;   // general IO
constexpr u32 GPIO_Serial = 0x01; // 1я функция
constexpr u32 GPIO_Timer = 0x02;  // 2я функция
constexpr u32 GPIO_Analog = 0x03; // 3я функция

constexpr u32 GPIO_Float = 0x00;  // floating
constexpr u32 GPIO_VCC = 0x04;    // pull-up
constexpr u32 GPIO_GND = 0x08;    // pull-down
constexpr u32 GPIO_Open = 0x0C;   // open-drain (не реализован)

constexpr u32 GPIO_2mA = 0x00;
constexpr u32 GPIO_4mA = 0x10;
constexpr u32 GPIO_8mA = 0x20;
constexpr u32 GPIO_max = 0x30;

// Для совместимости
constexpr u32 GPIO_50MHz = 0x00;
constexpr u32 GPIO_10MHz = 0x10;
constexpr u32 GPIO_2MHz = 0x20;

constexpr u32 GPIO_Imput = 0x00;  // Вход
constexpr u32 GPIO_Output = 0x40; // Выход

typedef struct {
  volatile uint32_t CFG;
  volatile uint32_t DS;
  volatile uint32_t PUPD;
} PAD_CONFIG_T;

template <uc32 N, uc32 P>
class Pin {
private:
  constexpr static PAD_CONFIG_T *PADx() { return  ((PAD_CONFIG_T *)PAD_CONFIG_BASE_ADDRESS) + N; }
  constexpr static GPIO_TypeDef *GPIOx()
  {
    switch (N) {
      case 0: return GPIO_0;
      case 1: return GPIO_1;
      case 2: return GPIO_2;
    }
    return 0;
  }

public:
  INLINE void set() { GPIOx()->SET = 1 << P; }
  INLINE void clr() { GPIOx()->CLEAR = 1 << P; }
  INLINE void inv() { GPIOx()->OUTPUT ^= 1 << P; }
  INLINE void out(bool data) { if (data) set(); else clr(); }
  INLINE u32 get() { return GPIOx()->STATE & (1 << P); }

  INLINE void in_analog() { init(GPIO_Analog | GPIO_Imput); }
  INLINE void in_nc() { init(GPIO_Port | GPIO_Float | GPIO_2mA); }
  INLINE void in_vcc() { init(GPIO_Port | GPIO_VCC | GPIO_2mA); }
  INLINE void in_gnd() { init(GPIO_Port | GPIO_GND | GPIO_2mA); }
  // INLINE void out(uc32 conf = GPIO_Port | GPIO_Float | GPIO_max) { init(conf | GPIO_Output); }

  INLINE void init(uc32 conf)
  {
    PADx()->CFG = (PADx()->CFG & ~PAD_CONFIG_PIN_M(P)) | PAD_CONFIG_PIN(P, conf & 0b11);
    PADx()->PUPD = (PADx()->PUPD & ~PAD_CONFIG_PIN_M(P)) | PAD_CONFIG_PIN(P, (conf >> 2) & 0b11);
    PADx()->DS = (PADx()->DS & ~PAD_CONFIG_PIN_M(P)) | PAD_CONFIG_PIN(P, conf >> 4);
    if (conf & GPIO_Output) GPIOx()->DIRECTION_OUT = 1 << P;
    else GPIOx()->DIRECTION_IN = 1 << P;
  }
};

template <uc32 N, uc32 PINS>
class Port {
private:
  constexpr static PAD_CONFIG_T *PADx() { return  ((PAD_CONFIG_T *)PAD_CONFIG_BASE_ADDRESS) + N; }
  constexpr static GPIO_TypeDef *GPIOx()
  {
    switch (N) {
      case 0: return GPIO_0;
      case 1: return GPIO_1;
      case 2: return GPIO_2;
    }
    return 0;
  }

public:
  INLINE u32 get() { return GPIOx()->STATE & PINS; }
  INLINE void out(u32 data) { GPIOx()->OUTPUT = data; }
  void init(uc32 conf = GPIO_Port | GPIO_Float | GPIO_2mA | GPIO_Imput)
  {
    for (u32 pin = 0; pin < 16; pin++) {
      PADx()->CFG = (PADx()->CFG & ~PAD_CONFIG_PIN_M(pin)) | PAD_CONFIG_PIN(pin, conf & 0b11);
      PADx()->PUPD = (PADx()->PUPD & ~PAD_CONFIG_PIN_M(pin)) | PAD_CONFIG_PIN(pin, (conf >> 2) & 0b11);
      PADx()->DS = (PADx()->DS & ~PAD_CONFIG_PIN_M(pin)) | PAD_CONFIG_PIN(pin, (conf >> 4) & 0b11);
    }

    if (conf & GPIO_Output) GPIOx()->DIRECTION_OUT = PINS;
    else GPIOx()->DIRECTION_IN = PINS;
  }
};
