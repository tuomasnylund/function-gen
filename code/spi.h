/*
* File: simple SPI routines for AVR
* Author: Tuomas Nylund (tuomas.nylund@gmail.com)
* Website: http://tuomasnylund.fi
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
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
