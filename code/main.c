/*
* File: main file for function generator
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

//standard libs
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//avr libs
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

//LUFA stuff
#include <LUFA/Version.h>
#include <LUFA/Drivers/USB/USB.h>
#include "Descriptors.h"

//own stuff
#include "spi.h"
#include "ad9833.h"

#define STRBUFLEN 15

/**********************************************************
 * Function prototypes
 **********************************************************/

void initialize(void);

//LUFA callbacks
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

/**********************************************************
 * External variables
 **********************************************************/

/**********************************************************
 * Global variables
 **********************************************************/

/** LUFA CDC Class driver interface configuration and state 
 *  information. This structure is passed to all CDC Class 
 *  driver functions, so that multiple instances of the same 
 *  class within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
    .Config =
    {
        .ControlInterfaceNumber         = 0,

        .DataINEndpointNumber           = CDC_TX_EPNUM,
        .DataINEndpointSize             = CDC_TXRX_EPSIZE,
        .DataINEndpointDoubleBank       = false,

        .DataOUTEndpointNumber          = CDC_RX_EPNUM,
        .DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
        .DataOUTEndpointDoubleBank      = false,

        .NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
        .NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
        .NotificationEndpointDoubleBank = false,
    },
};

/** Standard file stream for the CDC interface when set up,
 *  so that the virtual CDC COM port can be used like any
 *  regular character stream in the C APIs
 */
FILE USBSerialStream;

/**********************************************************
 * Main function
 **********************************************************/
int main(void){

    //variables
    //uint8_t i;
    char sBuffer[STRBUFLEN]; //!< character buffer for usb serial

    //initialize
    initialize();

    //main loop
    while (1)
    {
        /** Get input */
        fgets(sBuffer,STRBUFLEN,&USBSerialStream);

        if (sBuffer[0] == 's'){ //< Command is a set command
            switch (sBuffer[1]){
                case 'f':       //< frequency related
                    switch (sBuffer[2]){
                        case '1':
                            ad9833_set_frequency(0, atof(&(sBuffer[4])));
                            break;
                        case '2':
                            ad9833_set_frequency(1, atof(&(sBuffer[4])));
                            break;
                        case 'o':
                            switch (sBuffer[4]){
                                case '1':
                                    ad9833_set_freq_out(0);
                                    break;
                                case '2':
                                    ad9833_set_freq_out(1);
                                    break;
                                case 'm':
                                    ad9833_set_freq_out(2);
                                    break;
                            }
                            break;
                    }
                    break;
                case 'p': //< phase related
                    switch (sBuffer[2]){
                        case '1':
                            ad9833_set_phase(0, atof(&(sBuffer[4])));
                            break;
                        case '2':
                            ad9833_set_phase(1, atof(&(sBuffer[4])));
                            break;
                        case 'o':
                            switch (sBuffer[4]){
                                case '1':
                                    ad9833_set_phase_out(0);
                                    break;
                                case '2':
                                    ad9833_set_phase_out(1);
                                    break;
                                case 'm':
                                    ad9833_set_phase_out(2);
                                    break;
                            }
                            break;
                    }
                    break;
                case 'o': //<set output mode
                    switch (sBuffer[3]){
                        case 'o':
                            ad9833_set_mode(AD_OFF);
                            break;
                        case 's':
                            ad9833_set_mode(AD_SINE);
                            break;
                        case 't':
                            ad9833_set_mode(AD_TRIANGLE);
                            break;
                        case 'q':
                            ad9833_set_mode(AD_SQUARE);
                            break;
                    }
                    break;
                case 'm': //<set modulation freq
                    ad9833_set_mod_freq(atof(&(sBuffer[3])));
            }
        }
        else if (sBuffer[0] == 'g'){
        }
        
        /**LUFA usb related tasks*/
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();

    }
    return 0;
}

/**********************************************************
 * Interrupt vectors
 **********************************************************/


/**********************************************************
 * Other functions
 **********************************************************/

/** Initializes all of the hardware. */
void initialize(void){
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

    /* Init SPI */
    spi_init(SPIMODE2);

    /* Init AD9833 */
    ad9833_init();

    /** LUFA USB related inits */
	USB_Init();
	CDC_Device_CreateBlockingStream
        (&VirtualSerial_CDC_Interface, &USBSerialStream);


    /** enable interrupts*/
    sei();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void){
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void){
}

void EVENT_USB_Device_ConfigurationChanged(void){
    CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void){
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}
