/* 
 * File:   pragmas.c
 * Author: akashborde
 * 
 * Purpose: All the initializations needed for the pic 
 * 
 * Created on April 24, 2019, 5:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "peripheral.h"
#include <xc.h> //SFR definitions
#include <sys/attribs.h> // __ISR macro (not sure)

void initializePIC32() // initializes pic
{
   // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
}

void LED_Setup_A4()
{
     // Green LED pin:  A4
    TRISAbits.TRISA10 = 0; //set as output
    LATAbits.LATA10 = 1;   //initialized as ON
}

void LED_Invert_A4()
{
     LATAINV = 0b10000000000; //INVERT THE LED
}

