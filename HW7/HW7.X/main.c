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

#include "i2c_master_noint.h"
//#include "i2c_master_noint.h" //i2c initialization File

#define FIVE_HZ_PERIOD 4807692
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
// = a count every 0.0000000416 sec
//Desired period = 0.2s
//Count to 4807692




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
      //i2c_master_setup(); //initialize i2c2 register
    initExpander(); // initialize the i2c
    
    //5Hz LED initialize
    // Green LED pin:  A4
    TRISAbits.TRISA4 = 0; //set as output
    LATAbits.LATA4 = 0;   //initialized as low
    _CP0_SET_COUNT(0); //begin timer
    
    __builtin_enable_interrupts();
    
    char read = getExpander();
    
    while(1) 
    {
        
        if(read == WHOAMI_ID)
        {
            if (_CP0_GET_COUNT() > 4000000)
            {
                LATAINV = 0b10000; //INVERT LED
               _CP0_SET_COUNT(0);
            }
            //LATAbits.LATA4 = 1; //turn on LED
            
        }
        
        
       
//        char GP6_status = (read >> 6); //extract the first bit (GP7 info)
//        GP6_status &= 0b01; // clear first bit just incase there was info from bit 7        
//        
//        if (GP6_status)//read this input pin, which reflects the GP7 state
//        {
//            setExpander(0,1);
//        }
//        else
//        {        
//            setExpander(0,0);
//        }     
        
        
    
    }
}