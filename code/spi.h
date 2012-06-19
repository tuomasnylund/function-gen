/*
 * SPI MASTER TEST LIBRARY
 * -Tuomas Nylund
 */
#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

#define SPI_DDR      DDRB
#define SPI_MOSI_BIT PB2
#define SPI_SCK_BIT  PB1
#define SPI_CS_BIT   PB0

//init parameters
#define SPIMODE0   (0<<CPHA)
#define SPIMODE1   (1<<CPHA)
#define SPIMODE2   (2<<CPHA)
#define SPIMODE3   (3<<CPHA)

#define LSB_FIRST  (1<<DORD)
#define MSB_FIRST  (0<<DORD)

#define CLOCKDIV4  (0<<SPR0)
#define CLOCKDIV16 (1<<SPR0)
#define CLOCKDIV64 (2<<SPR0)

void spi_init(uint8_t setup);

void spi_send_byte(uint8_t data);

uint8_t spi_read_byte(void);

#endif
