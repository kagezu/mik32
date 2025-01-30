#include "avr/core.h"

#ifdef __AVR_ATmega328P__

#define SPI_SS(x)     x (B, PB2)
#define SPI_MOSI(x)   x (B, PB3)
#define SPI_MISO(x)   x (B, PB4)
#define SPI_SCK(x)    x (B, PB5)

#endif

#ifdef __AVR_ATmega128__

#define SPI_SS(x)     x (B, PB0)
#define SPI_SCK(x)    x (B, PB1)
#define SPI_MOSI(x)   x (B, PB2)
#define SPI_MISO(x)   x (B, PB3)

#endif

#define SPI_MASTER_PIN                                    \
SPI_SS(OUT);SPI_MOSI(OUT);SPI_SCK(OUT);SPI_MISO(IN);

#define SPI_SLAVE_PIN                                     \
SPI_SS(IN);SPI_MOSI(IN);SPI_SCK(IN);SPI_MISO(OUT);

#define SPI_STOP                                          \
SPI_MOSI(IN); SPI_SCK(IN); SPI_MISO(IN);SPCR = 0;
