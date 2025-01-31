#ifdef MIK32V2
#include "spi.h"
#include "mik32_hal_spi.h"

SPI_HandleTypeDef hspi;
SPI_Master SPI;

// Частота в кГц
void init(uint16_t fq, uint8_t mode)
{
  uint8_t sck = 0;

  if (fq >= F_CPU / 4000) sck = SPI_BAUDRATE_DIV4;
  else if (fq >= F_CPU / 8000) sck = SPI_BAUDRATE_DIV8;
  else if (fq >= F_CPU / 16000) sck = SPI_BAUDRATE_DIV16;
  else if (fq >= F_CPU / 32000) sck = SPI_BAUDRATE_DIV32;
  else if (fq >= F_CPU / 64000) sck = SPI_BAUDRATE_DIV64;
  else if (fq >= F_CPU / 128000) sck = SPI_BAUDRATE_DIV128;
  else sck = SPI_BAUDRATE_DIV256;

  hspi.Instance = SPI_1;
  hspi.Init.SPI_Mode = HAL_SPI_MODE_MASTER;
  hspi.Init.ThresholdTX = 4;
  hspi.Init.Decoder = SPI_DECODER_NONE;
  hspi.Init.ManualCS = SPI_MANUALCS_ON;
  hspi.Init.ChipSelect = SPI_CS_NONE;
  hspi.Init.BaudRateDiv = sck;

  // hspi.Init.CLKPhase = SPI_PHASE_OFF;
  // hspi.Init.CLKPolarity = SPI_POLARITY_LOW;

  hspi.Init.CLKPhase = mode & 0b00000001;
  hspi.Init.CLKPolarity = (mode & 0b00000010) >> 1;

  HAL_SPI_Init(&hspi);
}

void SPI_Master::end()
{
  HAL_SPI_Disable(&hspi);
}

void SPI_Master::send(uint8_t data)
{
  uint8_t rxByte = 0;
  HAL_StatusTypeDef SPI_Status = HAL_SPI_Exchange(&hspi, &data, &rxByte, 1, SPI_TIMEOUT_DEFAULT * 2);
  if (SPI_Status != HAL_OK)
    HAL_SPI_ClearError(&hspi);
}

uint8_t SPI_Master::transfer(uint8_t data)
{
  uint8_t rxByte = 0;
  HAL_StatusTypeDef SPI_Status = HAL_SPI_Exchange(&hspi, &data, &rxByte, 1, SPI_TIMEOUT_DEFAULT * 2);
  if (SPI_Status != HAL_OK)
    HAL_SPI_ClearError(&hspi);
  return rxByte;
}


void SPI_Master::send16(uint16_t data)
{
  uint8_t buf[2];
  buf[0] = (data >> 8) & 0xFF;
  buf[1] = data & 0xFF;
  HAL_StatusTypeDef SPI_Status = HAL_SPI_Exchange(&hspi, buf, buf, 2, SPI_TIMEOUT_DEFAULT * 2);
  if (SPI_Status != HAL_OK)
    HAL_SPI_ClearError(&hspi);
}

uint16_t SPI_Master::transfer16(uint16_t data)
{
  uint8_t buf[2];
  uint16_t rxVal = 0;
  buf[0] = (data >> 8) & 0xFF;
  buf[1] = data & 0xFF;
  HAL_StatusTypeDef SPI_Status = HAL_SPI_Exchange(&hspi, buf, buf, 2, SPI_TIMEOUT_DEFAULT * 2);
  if (SPI_Status != HAL_OK)
    HAL_SPI_ClearError(&hspi);
  rxVal = (((uint16_t)buf[0]) << 8) | buf[1];
  return rxVal;
}

#endif
