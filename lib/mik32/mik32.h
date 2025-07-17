#pragma once
// #include "riscv_csr_encoding.h"
#include "scr1_csr_encoding.h"
#include "scr1_specific.h"
#include "csr.h"

#include "mik32_memory_map.h"

#include "analog_reg.h"
#include "boot.h"
#include "crc.h"
#include "crypto.h"
#include "dma_config.h"
#include "eeprom.h"
#include "epic.h"
#include "gpio_irq.h"
#include "gpio.h"
#include "i2c.h"
#include "otp.h"
#include "pad_config.h"
#include "power_manager.h"
// #include "pvd_control.h"
#include "rtc.h"
#include "scr1_timer.h"
#include "spi.h"
#include "spifi.h"
#include "timer16.h"
#include "timer32.h"
#include "uart.h"
#include "wakeup.h"
#include "wdt_bus.h"
#include "wdt.h"

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
