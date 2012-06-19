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
