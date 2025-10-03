#include "gpio.h"

// USER ACE-UNO

Pin<PB, 0> USER_B;
Pin<PB, 5> USER_L;

// ADC

Pin<PC, 0> ADC0;
Pin<PC, 1> ADC1;
Pin<PC, 2> ADC2;
Pin<PC, 3> ADC3;
Pin<PC, 4> ADC4;
Pin<PC, 5> ADC5;
Pin<PC, 6> ADC6;
Pin<PC, 7> ADC7;

// DAC

Pin<PC, 6> DAC0;
Pin<PC, 7> DAC1;

// SPI

Pin<PB, 4> SPI0_MISO;
Pin<PB, 3> SPI0_MOSI;
Pin<PB, 5> SPI0_SCK;
Pin<PB, 2> SPI0_NSS_IN;
Pin<PB, 1> SPI0_NSS0;
Pin<PB, 0> SPI0_NSS1;
Pin<PB, 6> SPI0_NSS2;
Pin<PB, 7> SPI0_NSS3;

Pin<PB, 4> SPI1_MISO;
Pin<PB, 3> SPI1_MOSI;
Pin<PB, 5> SPI1_SCK;
Pin<PB, 2> SPI1_NSS_IN;
Pin<PB, 1> SPI1_NSS0;
Pin<PB, 0> SPI2_NSS1;
Pin<PB, 6> SPI3_NSS2;
Pin<PB, 7> SPI4_NSS3;

// I2C

Pin<PC, 4> I2C0_SDA;
Pin<PC, 5> I2C0_SCL;

Pin<PC, 4> I2C1_SDA;
Pin<PC, 5> I2C1_SCL;

// UART

Pin<PD, 0> UART0_RXD;
Pin<PD, 1> UART0_TXD;
Pin<PD, 4> UART0_CTS;
Pin<PD, 2> UART0_RTS;

Pin<PD, 0> UART1_RXD;
Pin<PD, 1> UART1_TXD;
Pin<PD, 4> UART1_CTS;
Pin<PD, 2> UART1_RTS;

//

Pin<PC, 7> AREF;
