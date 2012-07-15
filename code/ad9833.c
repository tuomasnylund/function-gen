/*
* File: code for controlling the ad9833 DDS
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

/**
 * \file ad9833.c
 *
 * Functions for controlling a AD9833 DDS chip with an AVR microcontroller
 * The chip allows generating sine, square and triangle waves at frequencies
 * ranging from ~0.1Hz to ~3MHz.
 *
 * It can also do some simple binary modulation in the form of FSK and PSK.
 * Amplitude modulation should be possible aswell, but isn't implemented yet.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ad9833.h"
#include "spi.h"

ad9833_settings_t ad_settings; ///<This is used to store all settings.

/**
 * a wrapper function for sending 16-bit SPI packets.
 * \param packet 16-bit value to be sent over SPI.
 */
static inline void ad9833_send(uint16_t packet){
    spi_send_byte((uint8_t)(packet>>8));
    spi_send_byte((uint8_t)packet);
}

/**
 * Initializes the AD9833 and the relevant variables.
 * Also initializes the Timer1 peripheral that is used for modulation timing.
 */
void ad9833_init(void){
    //init FSYNC pin (aka Chip select)
    ad_settings.command_reg |= (1<<AD_B28);
    AD_FSYNC_DDR |= (1<<AD_FSYNC_BIT);
    AD_FSYNC_HI();

    //init timer modulation
    TCCR1B |= (1<<WGM12);  //timer in CTC mode
    TCCR1B |= (1<<CS11)|(1<<CS10);//clockdiv 64
    TIMSK1 |= (1<<OCIE1A);
    OCR1A = 0xFFF0;

    //some datasheet-proscribed delay here
    _delay_us(10);

    //start as half-asleep
    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send((1<<AD_SLEEP12)|(1<<AD_RESET));
    ad_settings.command_reg |= (1<<AD_SLEEP12);
    _delay_us(5);
    AD_FSYNC_HI();

    //set some nice default values
    ad9833_set_frequency(0, 0);
    ad9833_set_frequency(1, 0);
    ad9833_set_phase(0, 0);
    ad9833_set_phase(1, 0);
    ad9833_set_freq_out(0);
    ad9833_set_phase_out(0);

}

/** 
 * Sets the ad9833 output waveform to the one given as a parameter.
 * \param mode possible values:
 *      - AD_OFF
 *      - AD_TRIANGLE
 *      - AD_SQUARE
 *      - AD_SINE
 */
void ad9833_set_mode(uint8_t mode){
    ad_settings.mode = mode;
    switch (mode){
        case AD_OFF:
            ad_settings.command_reg |= (1<<AD_SLEEP12);
            ad_settings.command_reg |= (1<<AD_SLEEP1);
            break;
        case AD_TRIANGLE:
            ad_settings.command_reg &= ~(1<<AD_OPBITEN);
            ad_settings.command_reg |=  (1<<AD_MODE);
            ad_settings.command_reg &= ~(1<<AD_SLEEP12);
            ad_settings.command_reg &= ~(1<<AD_SLEEP1);
            break;
        case AD_SQUARE:
            ad_settings.command_reg |=  (1<<AD_OPBITEN);
            ad_settings.command_reg &= ~(1<<AD_MODE);
            ad_settings.command_reg |=  (1<<AD_DIV2);
            ad_settings.command_reg &= ~(1<<AD_SLEEP12);
            ad_settings.command_reg &= ~(1<<AD_SLEEP1);
            break;
        case AD_SINE:
            ad_settings.command_reg &= ~(1<<AD_OPBITEN);
            ad_settings.command_reg &= ~(1<<AD_MODE);
            ad_settings.command_reg &= ~(1<<AD_SLEEP12);
            ad_settings.command_reg &= ~(1<<AD_SLEEP1);
            break;
    }

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send(ad_settings.command_reg);
    _delay_us(5);
    AD_FSYNC_HI();
}

/**
 * sets the desired ad9833 internal phase register to a value that
 * produces the desired phase.
 *
 * \param reg the desired phase register to be manipulated, either 0 or 1
 * \param phase the desired phase
 */
void ad9833_set_phase(uint8_t reg, double phase){
    uint16_t reg_reg; //probably should be renamed...
    if (reg==1)
        reg_reg = AD_PHASE1;
    else
        reg_reg = AD_PHASE0;

    ad_settings.phase[reg] = phase;

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send(reg_reg | AD_PHASE_CALC(ad_settings.phase[reg]));
    _delay_us(5);
    AD_FSYNC_HI();
}

/**
 * returns the phase of the selected register
 * \param reg the register of which value we want to get
 * \return the phase of the selected register
 */
double ad9833_get_phase(uint8_t reg){
    return ad_settings.phase[reg];
}

/**
 * Selects which frequency register is used to generate the output.
 * Also used to select FSK.
 * \param phase_out possible values:
 *      - 0 = use phase register 0
 *      - 1 = use phase register 1
 *      - 2 = PSK
 */
void    ad9833_set_freq_out(uint8_t freq_out){
    ad_settings.freq_out = freq_out;
    switch (freq_out){
        case 0:
            ad_settings.command_reg &= ~(1<<AD_FSELECT);
            break;
        case 1:
            ad_settings.command_reg |= (1<<AD_FSELECT);
            break;
        case 2:
            //TODO
            break;
    }

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send(ad_settings.command_reg);
    _delay_us(5);
    AD_FSYNC_HI();
}

/**
 * returns the previously set frequency output mode.
 * \return the previously set frequency out mode
 */
uint8_t ad9833_get_freq_out(void){
    return ad_settings.freq_out;
}

/**
 * Selects which phase register is used to generate the output
 * Also used to select PSK
 * \param phase_out possible values:
 *  - 0 = use phase register 0
 *  - 1 = use phase register 1
 *  - 2 = PSK
 */
void    ad9833_set_phase_out(uint8_t phase_out){
    ad_settings.phase_out = phase_out;
    switch (phase_out){
        case 0:
            ad_settings.command_reg &= ~(1<<AD_PSELECT);
            break;
        case 1:
            ad_settings.command_reg |= (1<<AD_PSELECT);
            break;
        case 2:
            //TODO
            break;
    }

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send(ad_settings.command_reg);
    _delay_us(5);
    AD_FSYNC_HI();
}

/**
 * returns the previously set phase output mode.
 * \return the previously set phase out mode
 */
uint8_t ad9833_get_phase_out(void){
    return ad_settings.phase_out;
}

/**
 * sets the desired ad9833 internal frequency register to a value that
 * produces the desired frequency.
 *
 * \param reg the desired frequency register to be manipulated, either 0 or 1
 * \param freq the desired frequency
 */
void ad9833_set_frequency(uint8_t reg, double freq){
    uint32_t freq_reg;
    uint16_t reg_reg; //probably should be renamed...
    freq_reg = AD_FREQ_CALC(freq);
    ad_settings.freq[reg] = freq;

    if (reg==1)
        reg_reg = AD_FREQ1;
    else
        reg_reg = AD_FREQ0;

    AD_FSYNC_LO();
    _delay_us(5);
    ad9833_send((1<<AD_B28) | ad_settings.command_reg);
    ad9833_send(reg_reg | (0x3FFF&(uint16_t)(freq_reg>>2 )));
    ad9833_send(reg_reg | (0x3FFF&(uint16_t)(freq_reg>>16)));
    _delay_us(5);
    AD_FSYNC_HI();
}

/**
 * returns the frequency of the selected register
 * \param reg the register of which value we want to get
 * \return the frequency of the selected register
 */
double ad9833_get_frequency(uint8_t reg){
    return ad_settings.freq[reg];
}

/**
 * sets the modulation frequenct to the desired value
 *
 * \param freq the desired modulation frequency
 */
void    ad9833_set_mod_freq(uint16_t freq){
    ad_settings.mod_freq = freq;
    OCR1A = AD_MOD_FREQ_CALC(freq);
}


/**
 * Sets the bytes to be modulated
 * NOT IMPLEMENTED YET
 *
 * \param num number of bytes to be sent
 * \param bytes pointer to an array of bytes to be sent
 * \param repeat should the sending be repeated
 */
void    ad9833_set_mod_bytes(uint8_t num, uint8_t *bytes, uint8_t repeat){
    //TODO implements this thing
}

/**
 * Timer interrupt for handling modulation
 */
ISR(TIMER1_COMPA_vect){
    uint16_t check = ad_settings.command_reg;
    //TODO implement modulation of real signals
    if (ad_settings.freq_out  == 2)
        ad_settings.command_reg ^= ((uint16_t)1<<AD_FSELECT);
    if (ad_settings.phase_out == 2)
        ad_settings.command_reg ^= ((uint16_t)1<<AD_PSELECT);

    if (check != ad_settings.command_reg){
        AD_FSYNC_LO();
        _delay_us(5);
        ad9833_send(ad_settings.command_reg);
        _delay_us(5);
        AD_FSYNC_HI();
    }
}
