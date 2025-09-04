#pragma once
// #include "riscv_csr_encoding.h"
#include "inc/scr1_csr_encoding.h"
#include "inc/scr1_specific.h"
#include "inc/csr.h"
#include "inc/mik32_memory_map.h"
#include "inc/analog_reg.h"
#include "inc/boot.h"
#include "inc/crc.h"
#include "inc/crypto.h"
#include "inc/dma_config.h"
#include "inc/eeprom.h"
#include "inc/epic.h"
#include "inc/gpio_irq.h"
#include "inc/gpio.h"
#include "inc/i2c.h"
#include "inc/otp.h"
#include "inc/pad_config.h"
#include "inc/power_manager.h"
#include "inc/pvd_control.h"
#include "inc/rtc.h"
#include "inc/scr1_timer.h"
#include "inc/spi.h"
#include "inc/spifi.h"
#include "inc/timer16.h"
#include "inc/timer32.h"
#include "inc/uart.h"
#include "inc/wakeup.h"
#include "inc/wdt_bus.h"
#include "inc/wdt.h"

// MIK32 + ACE UNO
#include "pinout.h"

// Системная частота
#ifndef F_CPU
#define F_CPU   OSC_SYSTEM_VALUE
#endif

// Доступ к байтам
// #define to_byte(w,x)  ((uint8_t)(w >> (x << 3)))
// #define to_dbyte(w,x)  ((uint16_t)(w >> (x << 4)))

#include "macros/attribute.h"
#include "macros/gpio.h"
#include "macros/pgmspace.h"
