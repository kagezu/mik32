// #include "pinout.h"
#pragma once

// USER ACE-UNO

#define USER_B(f)       f(B, 0)
#define USER_L(f)       f(B, 5)

// ADC

#define ADC0(f)         f(C, 0)
#define ADC1(f)         f(C, 1)
#define ADC2(f)         f(C, 2)
#define ADC3(f)         f(C, 3)
#define ADC4(f)         f(C, 4)
#define ADC5(f)         f(C, 5)
#define ADC6(f)         f(C, 6)
#define ADC7(f)         f(C, 7)

// DAC

#define DAC0(f)         f(C, 6)
#define DAC1(f)         f(C, 7)

// SPI

#define SPI0_MISO(f)    f (B, 4)
#define SPI0_MOSI(f)    f (B, 3)
#define SPI0_SCK(f)     f (B, 5)
#define SPI0_NSS_IN(f)  f (B, 2)
#define SPI0_NSS0(f)    f (B, 1)
#define SPI0_NSS1(f)    f (B, 0)
#define SPI0_NSS2(f)    f (B, 6)
#define SPI0_NSS3(f)    f (B, 7)

#define SPI1_MISO(f)    f (B, 4)
#define SPI1_MOSI(f)    f (B, 3)
#define SPI1_SCK(f)     f (B, 5)
#define SPI1_NSS_IN(f)  f (B, 2)
#define SPI1_NSS0(f)    f (B, 1)
#define SPI2_NSS1(f)    f (B, 0)
#define SPI3_NSS2(f)    f (B, 6)
#define SPI4_NSS3(f)    f (B, 7)

// I2C

#define I2C0_SDA(f)     f (C, 4)
#define I2C0_SCL(f)     f (C, 5)

#define I2C1_SDA(f)     f (C, 4)
#define I2C1_SCL(f)     f (C, 5)

// UART

#define UART0_RXD(f)    f (D, 0)
#define UART0_TXD(f)    f (D, 1)
#define UART0_CTS(f)    f (D, 4)
#define UART0_RTS(f)    f (D, 2)

#define UART1_RXD(f)    f (D, 0)
#define UART1_TXD(f)    f (D, 1)
#define UART1_CTS(f)    f (D, 4)
#define UART1_RTS(f)    f (D, 2)

//

#define AREF(f)         f (C, 7)
