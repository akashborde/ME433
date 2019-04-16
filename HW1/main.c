/* 
 * File:   main.c
 * Author: akashborde
 *
 * Created on April 16, 2019, 3:16 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

//values for #pragma config settings are found in:
// /opt/microchip/xc32/v2.15/docs/config_docs

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25  // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV, 4*24 = 96MHz
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL, 96/2 = 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 00000000 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

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
    LATAbits.LATA4 = 1;   //initialized as high
    
    __builtin_enable_interrupts();
    
     _CP0_SET_COUNT(0); //begin timer

    while(1) 
    {
        
        //if button pushed, turn LED off.
        if (PORTBbits.RB4 == 0) //read this input pin
        {
            LATAbits.LATA4 = 0;  //turn off the LED
        }
        else
        {        
            // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
            // remember the core timer runs at half the sysclk

            //LATAbits.LATA4 = 1;   //turn LED on

            if (_CP0_GET_COUNT() > HALF_MILLISEC_IN_COUNTS)
            {
                LATAINV = 0b10000;   //invert LED
                 _CP0_SET_COUNT(0); //reset timer
            }
        }     
    
    }
}