#include <avr/io.h>
#include <util/delay.h>
#include "ad9833.h"
#include "spi.h"


uint16_t ad_cmd_reg;


void ad9833_send(uint16_t packet){
    spi_send_byte((uint8_t)(packet>>8));
    spi_send_byte((uint8_t)packet);
}

void ad9833_init(void){
    ad_cmd_reg |= (1<<AD_B28);
    AD_FSYNC_DDR |= (1<<AD_FSYNC_BIT);

    AD_FSYNC_HI();

    _delay_us(10);

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send((1<<AD_B28)|(1<<AD_RESET));
    _delay_us(5);
    AD_FSYNC_HI();

    _delay_us(10);
    ad9833_set_frequency(AD_FREQ0, 0);
    _delay_us(10);
    ad9833_set_frequency(AD_FREQ1, 0);
    _delay_us(10);
    ad9833_set_phase(AD_PHASE0, 0);
    _delay_us(10);
    ad9833_set_phase(AD_PHASE1, 0);
    _delay_us(10);

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send((1<<AD_B28));
    _delay_us(5);
    AD_FSYNC_HI();
}

void ad9833_power(uint8_t power){
}

void ad9833_set_type(uint8_t type){
    switch (type){
        case AD_TRIANGLE:
            ad_cmd_reg &= ~(1<<AD_OPBITEN);
            ad_cmd_reg |=  (1<<AD_MODE);
            break;
        case AD_SQUARE:
            ad_cmd_reg |=  (1<<AD_OPBITEN);
            ad_cmd_reg &= ~(1<<AD_MODE);
            break;
        case AD_SINE:
            ad_cmd_reg &= ~((1<AD_OPBITEN)|(1<<AD_MODE));
            break;
    }

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send((1<<AD_B28) | ad_cmd_reg);
    _delay_us(5);
    AD_FSYNC_HI();

}

void ad9833_set_phase(uint16_t reg, uint16_t phase){

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send(reg | phase);
    _delay_us(5);
    AD_FSYNC_HI();
}
void ad9833_set_frequency(uint16_t reg, uint32_t freq){

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send((1<<AD_B28) | ad_cmd_reg);
    ad9833_send(reg | (0x3FFF&(uint16_t)(freq>>2 )));
    ad9833_send(reg | (0x3FFF&(uint16_t)(freq>>16)));
    _delay_us(5);
    AD_FSYNC_HI();
}
