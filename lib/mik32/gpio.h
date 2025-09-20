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


constexpr u32 GP_IO = 0x00;     // general IO
constexpr u32 GP_Serial = 0x01; // 1я функция
constexpr u32 GP_Timer = 0x02;  // 2я функция
constexpr u32 GP_Analog = 0x03; // 3я функция

constexpr u32 GP_Float = 0x00;  // floating
constexpr u32 GP_VCC = 0x04;    // pull-up
constexpr u32 GP_GND = 0x08;    // pull-down
constexpr u32 GP_Open = 0x00;   // open-drain (не реализован)

constexpr u32 GPO_2mA = 0x40;
constexpr u32 GPO_4mA = 0x50;
constexpr u32 GPO_8mA = 0x60;
constexpr u32 GPO_Max = 0x70;

// Для совместимости
constexpr u32 GPO_50MHz = 0x40;
constexpr u32 GPO_10MHz = 0x50;
constexpr u32 GPO_2MHz = 0x60;


typedef struct {
  volatile uint32_t CFG;
  volatile uint32_t DS;
  volatile uint32_t PUPD;
} PAD_CONFIG_T;

template <uc32 N, uc32 PINx>
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
  INLINE void set() { GPIOx()->SET = 1 << PINx; }
  INLINE void clr() { GPIOx()->CLEAR = 1 << PINx; }
  INLINE void inv() { GPIOx()->OUTPUT ^= 1 << PINx; }
  INLINE void out(bool data) { if (data) set(); else clr(); }
  INLINE u32 get() { return GPIOx()->STATE & (1 << PINx); }
  INLINE void init(uc32 conf = GP_IO)
  {
    PADx()->CFG = (PADx()->CFG & ~PAD_CONFIG_PIN_M(PINx)) | PAD_CONFIG_PIN(PINx, conf & 0b11);
    PADx()->PUPD = (PADx()->PUPD & ~PAD_CONFIG_PIN_M(PINx)) | PAD_CONFIG_PIN(PINx, (conf >> 2) & 0b11);
    PADx()->DS = (PADx()->DS & ~PAD_CONFIG_PIN_M(PINx)) | PAD_CONFIG_PIN(PINx, conf >> 4);
    if (conf & GPO_Max) GPIOx()->DIRECTION_OUT = 1 << PINx;
    else GPIOx()->DIRECTION_IN = 1 << PINx;
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
  INLINE void set(u32 data) { GPIOx()->SET = data; }
  INLINE void clr(u32 data) { GPIOx()->CLEAR = data; }
  INLINE void inv(u32 data) { GPIOx()->OUTPUT ^= data; }
  INLINE void out(u32 data) { GPIOx()->OUTPUT = data; }
  INLINE u32 get() { return GPIOx()->STATE; }
  void init(uc32 conf = GP_IO)
  {
    for (u32 pin = 0; pin < 16; pin++) {
      PADx()->CFG = (PADx()->CFG & ~PAD_CONFIG_PIN_M(pin)) | PAD_CONFIG_PIN(pin, conf & 0b11);
      PADx()->PUPD = (PADx()->PUPD & ~PAD_CONFIG_PIN_M(pin)) | PAD_CONFIG_PIN(pin, (conf >> 2) & 0b11);
      PADx()->DS = (PADx()->DS & ~PAD_CONFIG_PIN_M(pin)) | PAD_CONFIG_PIN(pin, (conf >> 4) & 0b11);
    }
    if (conf & GPO_Max) GPIOx()->DIRECTION_OUT = PINS;
    else GPIOx()->DIRECTION_IN = PINS;
  }
};
