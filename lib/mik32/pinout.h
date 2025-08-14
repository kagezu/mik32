// #include "pinout.h"
#pragma once


// ADC

#define ADC0(f)         f(1, 5)
#define ADC1(f)         f(1, 7)
#define ADC2(f)         f(0, 2)
#define ADC3(f)         f(0, 4)
#define ADC4(f)         f(0, 7)
#define ADC5(f)         f(0, 9)
#define ADC6(f)         f(0, 11)
#define ADC7(f)         f(0, 13)

// DAC

#define DAC0(f)         f(1, 12)
#define DAC1(f)         f(1, 13)

// Aref

#define AREF(f)         f (1, 11)

// SPI

#define SPI0_MISO(f)    f(0, 0)
#define SPI0_MOSI(f)    f(0, 1)
#define SPI0_SCK(f)     f(0, 2)
#define SPI0_NSS_IN(f)  f(0, 3)
#define SPI0_NSS0(f)    f(0, 4)
#define SPI0_NSS1(f)    f(1, 14)
#define SPI0_NSS2(f)    f(1, 15)
#define SPI0_NSS3(f)    f(2, 6)

#define SPI1_MISO(f)    f(1, 0)
#define SPI1_MOSI(f)    f(1, 1)
#define SPI1_SCK(f)     f(1, 2)
#define SPI1_NSS_IN(f)  f(1, 3)
#define SPI1_NSS0(f)    f(1, 4)
#define SPI2_NSS1(f)    f(1, 5)
#define SPI3_NSS2(f)    f(1, 6)
#define SPI4_NSS3(f)    f(1, 7)

// JTAG

#define TDI(f)          f(0, 11)
#define TCK(f)          f(0, 12)
#define TMS(f)          f(0, 13)
#define TRST(f)         f(0, 14)
#define TDO(f)          f(0, 15)

// I2C

#define I2C0_SDA(f)     f(0, 9)
#define I2C0_SCL(f)     f(0, 10)

#define I2C1_SDA(f)     f(1, 12)
#define I2C1_SCL(f)     f(1, 13)

// UART

#define UART0_RXD(f)    f(0, 5)
#define UART0_TXD(f)    f(0, 6)
#define UART0_CTS(f)    f(0, 7)
#define UART0_RTS(f)    f(0, 8)

#define UART1_RXD(f)    f(1, 8)
#define UART1_TXD(f)    f(1, 9)
#define UART1_CTS(f)    f(1, 10)
#define UART1_RTS(f)    f(1, 11)


// ACE-UNO

#define USER_B(f)       f(2, 6)
#define USER_L(f)       f(2, 7)

#define D0(f)           f(0, 5)
#define D1(f)           f(0, 6)
#define D2(f)           f(0, 10)
#define D3(f)           f(0, 0)
#define D4(f)           f(0, 8)
#define D5(f)           f(0, 1)
#define D6(f)           f(0, 2)
#define D7(f)           f(1, 8)
#define D8(f)           f(1, 9)
#define D9(f)           f(0, 3)
#define D10(f)          f(1, 3)
#define D11(f)          f(1, 1)
#define D12(f)          f(1, 0)
#define D13(f)          f(1, 2)

#define D18(f)          f(1, 12)
#define D19(f)          f(1, 13)

#define A0(f)           f(1, 5)
#define A1(f)           f(1, 7)
#define A2(f)           f(0, 4)
#define A3(f)           f(0, 7)
#define A45(f)          f(0, 9)

#define SELA45(f)       f(1, 15)
#define NSS0(f)         f(1, 10)
#define NSS1(f)         f(1, 6)
