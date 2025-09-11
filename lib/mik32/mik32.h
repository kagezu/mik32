#pragma once
// #include "riscv_csr_encoding.h"
#include "sdk/scr1_csr_encoding.h"
#include "sdk/scr1_specific.h"
#include "sdk/csr.h"
#include "sdk/mik32_memory_map.h"
#include "sdk/analog_reg.h"
#include "sdk/boot.h"
#include "sdk/crc.h"
#include "sdk/crypto.h"
#include "sdk/dma_config.h"
#include "sdk/eeprom.h"
#include "sdk/epic.h"
#include "sdk/gpio_irq.h"
#include "sdk/gpio.h"
#include "sdk/i2c.h"
#include "sdk/otp.h"
#include "sdk/pad_config.h"
#include "sdk/power_manager.h"
#include "sdk/pvd_control.h"
#include "sdk/rtc.h"
#include "sdk/scr1_timer.h"
#include "sdk/spi.h"
#include "sdk/spifi.h"
#include "sdk/timer16.h"
#include "sdk/timer32.h"
#include "sdk/uart.h"
#include "sdk/wakeup.h"
#include "sdk/wdt_bus.h"
#include "sdk/wdt.h"

// MIK32 + ACE UNO
#include "pinout.h"

// Системная частота
#ifndef F_CPU
#define F_CPU   OSC_SYSTEM_VALUE
#endif

#include "attribute.h"
#include "gpio.h"
#include "pgmspace.h"

#include "type/int.h"
