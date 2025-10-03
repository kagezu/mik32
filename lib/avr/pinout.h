// #include "pinout.h"
#pragma once
#include "gpio.h"

// USER ACE-UNO

extern Pin<PB, 0> USER_B;
extern Pin<PB, 5> USER_L;

// ADC

extern Pin<PC, 0> ADC0;
extern Pin<PC, 1> ADC1;
extern Pin<PC, 2> ADC2;
extern Pin<PC, 3> ADC3;
extern Pin<PC, 4> ADC4;
extern Pin<PC, 5> ADC5;
extern Pin<PC, 6> ADC6;
extern Pin<PC, 7> ADC7;

// DAC

extern Pin<PC, 6> DAC0;
extern Pin<PC, 7> DAC1;

// SPI

extern Pin<PB, 4> SPI0_MISO;
extern Pin<PB, 3> SPI0_MOSI;
extern Pin<PB, 5> SPI0_SCK;
extern Pin<PB, 2> SPI0_NSS_IN;
extern Pin<PB, 1> SPI0_NSS0;
extern Pin<PB, 0> SPI0_NSS1;
extern Pin<PB, 6> SPI0_NSS2;
extern Pin<PB, 7> SPI0_NSS3;

extern Pin<PB, 4> SPI1_MISO;
extern Pin<PB, 3> SPI1_MOSI;
extern Pin<PB, 5> SPI1_SCK;
extern Pin<PB, 2> SPI1_NSS_IN;
extern Pin<PB, 1> SPI1_NSS0;
extern Pin<PB, 0> SPI2_NSS1;
extern Pin<PB, 6> SPI3_NSS2;
extern Pin<PB, 7> SPI4_NSS3;

// I2C

extern Pin<PC, 4> I2C0_SDA;
extern Pin<PC, 5> I2C0_SCL;

extern Pin<PC, 4> I2C1_SDA;
extern Pin<PC, 5> I2C1_SCL;

// UART

extern Pin<PD, 0> UART0_RXD;
extern Pin<PD, 1> UART0_TXD;
extern Pin<PD, 4> UART0_CTS;
extern Pin<PD, 2> UART0_RTS;

extern Pin<PD, 0> UART1_RXD;
extern Pin<PD, 1> UART1_TXD;
extern Pin<PD, 4> UART1_CTS;
extern Pin<PD, 2> UART1_RTS;

//

extern Pin<PC, 7> AREF;
