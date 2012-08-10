/**
* \mainpage
* \section SecIntro Introduction
* This is the documentation for the microcontroller code for a function generator based on a 
* relatively cheap DDS chip. The board appears as a virtual serial port 
* to the computer. By sending certain strings of characters, you can configure the output
* of the function generator to do what you want.
*
* The library utilizes the LUFA USB library, but that is not documented in here. 
*
* \section Hardware
*  - Atmel at90usb162 microcontroller
*  - AD9833 DDS
*  - OPA357 opamp
*
* \section SecFeatures Features
*  - Sine, Square and Triangle waveforms
*  - 0.01Hz - 3MHz output frequency
*  - Binary Phase/Frequency shift keying
*
* \section SecFiles Files
* The important files of this project:
*  - \ref spi_h
*  - \ref ad9833_h
*  - \ref main_c
*
* \section license License
* All code is released under GPLv3
*/
