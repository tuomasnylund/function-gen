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
#include "spi.h"


void spi_init(uint8_t setup){
    SPI_DDR |= (1<<SPI_MOSI_BIT)|(1<<SPI_SCK_BIT)|(1<<SPI_CS_BIT);

    SPCR     = (1<<SPE)|(1<<MSTR)|setup;
}

void spi_send_byte(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_read_byte(void){
    SPDR = 0x00;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}
