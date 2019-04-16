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

//SYSCLK = 48MHz, Core Timer = 24MHz, so one count every 0.0000416 millisec
//Desired period = 0.5ms
//Count to 12019 to get to 0.5ms

#define HALF_MILLISEC_IN_COUNTS 12019

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

    // do your TRIS and LAT commands here
    
    /*
     * Add initializations in main() to make the pushbutton pin an input pin,
     * and the LED pin as an output that is initially high. 
     * Upload the code and verify that the LED turns on. 
     */
    
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