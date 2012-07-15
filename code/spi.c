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

/** \file spi.c
 * Simple code for using the AVR SPI peripharal, uses polling
 */

#include "spi.h"


/**
 * Initializes the AVR SPI peripheral according to the given parameters. 
 * \param setup SPI options, has to be combined mask of one of SPIMODE*, *_FIRST, CLOCKDIV* each
 */
void spi_init(uint8_t setup){
    SPI_DDR |= (1<<SPI_MOSI_BIT)|(1<<SPI_SCK_BIT)|(1<<SPI_CS_BIT);

    SPCR     = (1<<SPE)|(1<<MSTR)|setup;
}

/**
 * Sends one byte over SPI, uses polling. 
 * \param data byte to be sent
 */
void spi_send_byte(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

/**
 * Receives one byte over SPI, uses polling. 
 * \return received byte
 */
uint8_t spi_read_byte(void){
    SPDR = 0x00;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}
