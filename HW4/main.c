/* 
 * File:   main.c
 * Author: akashborde
 * Advanced Mechatronics, ME433 at Northwestern University
 * SPI Communication with PIC32
 * 
 * Created on April 16, 2019, 3:16 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "pragmas.h"
#include "spi.h"


int main() {

    __builtin_disable_interrupts();
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // Pushbutton pin: B4
    TRISBbits.TRISB4 = 1; // Set it as an input
   // Green LED pin:  A4
    TRISAbits.TRISA4 = 0; //set as output
    LATAbits.LATA4 = 1;   //initialized as ON
    
    __builtin_enable_interrupts();
    
    while(1) 
    {
        //if button pushed, turn LED off.
        if (PORTBbits.RB4 == 0) //read this input pin
        {
            LATAbits.LATA4 = 0;  //turn OFF the LED
        }
        else
        {        
            LATAbits.LATA4 = 1; // turn it on
        }     
    }
}