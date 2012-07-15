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

/** \file spi.h
 * Header file for spi.c
 */

#ifndef SPI_H_
#define SPI_H_

/** \defgroup spi_h spi.h - AVR SPI library
 * This is a very simple SPI library that uses the internal SPI peripheral of the AVR.
 *
 * It does not rely on interrupts, instead it uses polling.
 *
 * Any chip select lines or similiar things you have to implement yourself.
 *
 *
 * \section example Example
 * \code
 *      //initialize the SPI
 *      spi_init(SPIMODE0|MSB_FIRST|CLOCKDIV4);
 *      
 *      //Send a byte
 *      spi_send_byte(0xAA);
 *
 *      //Receive byte
 *      uint8_t rcv;
 *      rcv = spi_read_byte();
 * \endcode
 *
 * @{ */

#include <avr/io.h>

/** \name SPI pin definitions
 * @{ */
#define SPI_DDR      DDRB
#define SPI_MOSI_BIT PB2
#define SPI_SCK_BIT  PB1
#define SPI_CS_BIT   PB0
/** @} */

/** \name SPI mode masks
 * use with \ref spi_init()
 * @{ */
#define SPIMODE0   (0<<CPHA)
#define SPIMODE1   (1<<CPHA)
#define SPIMODE2   (2<<CPHA)
#define SPIMODE3   (3<<CPHA)
/** @} */

/** \name SPI bit order masks
 * use with \ref spi_init()
 * @{ */
#define LSB_FIRST  (1<<DORD)
#define MSB_FIRST  (0<<DORD)
/** @} */

/** \name SPI clock speed masks
 * use with \ref spi_init()
 * @{ */
#define CLOCKDIV4  (0<<SPR0)
#define CLOCKDIV16 (1<<SPR0)
#define CLOCKDIV64 (2<<SPR0)
/** @} */

void spi_init(uint8_t setup);

void spi_send_byte(uint8_t data);

uint8_t spi_read_byte(void);

#endif

/** @} */
