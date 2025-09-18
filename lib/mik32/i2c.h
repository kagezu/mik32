#pragma once
#include "core.h"
// #include "pinout.h"
// #include "mik32_hal_i2c.h"

extern "C" {
#include "mik32_hal_i2c.h"
}

#define I2C_IDLE    0x00
#define I2C_ENABLE  0x01


#define I2C_BUFFER_LENGTH 600

#define I2C_N (( I2C_TypeDef *)( I2C_0_BASE_ADDRESS + I2CN * 0x400 ))

template<const uint32_t I2CN = 1>
class I2C {
private:
  uint8_t state;
  uint8_t address;
  uint16_t index;
  uint8_t buffer[I2C_BUFFER_LENGTH];

  I2C_HandleTypeDef hi2c;

public:
  void set_address(uint8_t adr) { address = adr; }
  void set_freq(uint32_t freq)
  {
    hi2c.Clock.PRESC = freq;
    HAL_I2C_Init(&hi2c);
  }

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
    hi2c.Clock.PRESC = 0;
    hi2c.Clock.SCLDEL = 1;
    hi2c.Clock.SDADEL = 1;
    hi2c.Clock.SCLH = 0;
    hi2c.Clock.SCLL = 0;

    /* Настройка частоты */
    // hi2c.Clock.PRESC = 0;
    // hi2c.Clock.SCLDEL = 1;
    // hi2c.Clock.SDADEL = 1;
    // hi2c.Clock.SCLH = 5;
    // hi2c.Clock.SCLL = 5;

    HAL_I2C_Init(&hi2c);

    // state = I2C_IDLE;
  }


  void begin()
  {
    // state = I2C_ENABLE;
    index = 0;
  }

  void write(uint8_t data) { buffer[index++] = data; }

  void write(uint8_t *data, uint16_t len)
  {
    // if (index) HAL_I2C_Master_Transmit(&hi2c, address >> 1, buffer, index, I2C_TIMEOUT_DEFAULT);

    // HAL_I2C_Master_Transmit(&hi2c, address >> 1, data, len, I2C_TIMEOUT_DEFAULT);

    // index = 0;


    while (len--) buffer[index++] = *data++;
  }

  void end()
  {

    HAL_I2C_Master_Transmit(&hi2c, address >> 1, buffer, index, I2C_TIMEOUT_DEFAULT);

    if (hi2c.Init.AutoEnd == I2C_AUTOEND_DISABLE) {
      hi2c.Instance->CR2 |= I2C_CR2_STOP_M;
    }

    index = 0;

    // state = I2C_IDLE;
  }

  void test()
  {
    uint8_t b = 0;
    while (HAL_OK != HAL_I2C_Master_Transmit(&hi2c, address >> 1, &b, 1, I2C_TIMEOUT_DEFAULT))
      hi2c.Instance->CR2 |= I2C_CR2_STOP_M;

  }

  // static void send_address();
  // static void stop(void);
  // static void send_next_byte(void);

  // void destroy();

};
