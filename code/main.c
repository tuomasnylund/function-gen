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

#define STRBUFLEN 20

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

        if(sBuffer[0] == 'f'){
            ad9833_set_frequency(AD_FREQ0, atof(&(sBuffer[1])));
            fputs("frequency changed\r\n",&USBSerialStream);
        }

        else if(sBuffer[0] == 'l'){
            ad9833_set_mode(AD_SQUARE);
            fputs("square wave\r\n",&USBSerialStream);
        }

        else if(sBuffer[0] == 's'){
            ad9833_set_mode(AD_SINE);
            fputs("sine wave\r\n",&USBSerialStream);
        }

        else if(sBuffer[0] == 't'){
            ad9833_set_mode(AD_TRIANGLE);
            fputs("triangle wave\r\n",&USBSerialStream);
        }

        else if(sBuffer[0] == 'p'){
            ad9833_power(0);
            fputs("powered off\r\n",&USBSerialStream);
        }

        else if(sBuffer[0] == 'P'){
            ad9833_power(1);
            fputs("powered on\r\n",&USBSerialStream);
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
