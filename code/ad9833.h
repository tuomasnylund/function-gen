#ifndef _AD9833_H_
#define _AD9833_H_

#define AD_FSYNC_PORT PORTB
#define AD_FSYNC_DDR  DDRB
#define AD_FSYNC_BIT  PB0

#define AD_FSYNC_HI() AD_FSYNC_PORT |=  (1<<AD_FSYNC_BIT)
#define AD_FSYNC_LO() AD_FSYNC_PORT &= ~(1<<AD_FSYNC_BIT)

#define AD_TRIANGLE 0
#define AD_SQUARE   1
#define AD_SINE     2


#define AD_B28     13
#define AD_HLB     12
#define AD_FSELECT 11
#define AD_PSELECT 10
#define AD_RESET   8
#define AD_SLEEP1  7
#define AD_SLEEP12 6
#define AD_OPBITEN 5
#define AD_DIV2    3
#define AD_MODE    1

/**to send a freq or phase to correct register, OR the packet with one of these */
#define AD_FREQ0  (1<<14)
#define AD_FREQ1  (1<<15)
#define AD_PHASE0 (3<<14)
#define AD_PHASE1 ((3<<14)|(1<<13))


void ad9833_init(void);

void ad9833_set_type(uint8_t type);

void ad9833_power(uint8_t power);

void ad9833_set_frequency(uint16_t reg, uint32_t freq);

void ad9833_set_phase(uint16_t reg, uint16_t phase);


#endif
