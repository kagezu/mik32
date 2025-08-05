#pragma once
#include "core.h"
#include "pinout.h"

#include "mik32_hal_i2c.h"

#define TWI_FREQ 400000
#define TWI_BUFFER_LENGTH 32

#define I2C_N (( I2C_TypeDef *)( I2C_0_BASE_ADDRESS + I2CN * 0x400 ))

template<const uint32_t I2CN = 1>
class I2C {
private:
  uint8_t address;
  uint8_t _buffer[TWI_BUFFER_LENGTH];

  I2C_HandleTypeDef hi2c;

public:
  void init()
  {
    PM->CLK_APB_P_SET =
      PM_CLOCK_APB_P_I2C_0_M |
      PM_CLOCK_APB_P_I2C_1_M;

    /* Общие настройки */
    hi2c.Instance = I2C_1;
    hi2c.Init.Mode = HAL_I2C_MODE_MASTER;
    hi2c.Init.DigitalFilter = I2C_DIGITALFILTER_OFF;
    hi2c.Init.AnalogFilter = I2C_ANALOGFILTER_DISABLE;
    hi2c.Init.AutoEnd = I2C_AUTOEND_ENABLE;

    /* Настройка частоты */
    hi2c.Clock.PRESC = 1;
    hi2c.Clock.SCLDEL = 1;
    hi2c.Clock.SDADEL = 1;
    hi2c.Clock.SCLH = 1;
    hi2c.Clock.SCLL = 1;

    HAL_I2C_Init(&hi2c0);

  }




















  static void send_address();
  static void stop(void);
  static void send_next_byte(void);

  void destroy();
  void set_freq(uint32_t freq);
  void write(uint8_t *, uint16_t);
  void write(uint8_t);
  void begin();

  void set_address(uint8_t adr) { address = adr; }
  void end(void);
};
