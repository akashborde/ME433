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
#include <math.h>

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
    
    //spi chip select
    LATBbits.LATB15 = 1; //initialize as high (off)
    
    initSPI1();
    
    __builtin_enable_interrupts();

    _CP0_SET_COUNT(0);//begin timer
    int time_ms = 0;
    
    while(1) 
    {
        
        if (_CP0_GET_COUNT() > CPO_COUNTS_TO_1kHz) //enter loop every 1kHz = 1 ms
        {
            //want 10Hz sine wave, so T = 2pi/f = 2pi/10
            setVoltage(CHANNEL_A, SIN_VERTICAL_TRANSLATION + sin( 2*PI*FREQ_HZ*time_ms/SEC_TO_MSEC ) );
            
            //triangle function: y = x for x from 0 to T/2; 1-x from T/2 to T
            //T = 0.2 sec for 5Hz. If each count is 0.001sec,
            //T = 200 counts. So T/2 = 100 counts
            //To normalize the height of triangle to 2, divide by the expected max counts = 100 and multiply by 2
            
            //normalized to 1 period of the triangle wave
            int normal_time_ms = time_ms % TRIANGLE_PERIOD_COUNTS; 
            
            if( (normal_time_ms) < TRIANGLE_PERIOD_COUNTS/2) // first half of triangle
            {
                //output = 2 * input * 
                setVoltage(CHANNEL_B, TRIANGLE_HEIGHT * ((double) normal_time_ms * FREQ_HZ / SEC_TO_MSEC ) );
            }
            else if( normal_time_ms >= TRIANGLE_PERIOD_COUNTS/2)
            {
                setVoltage(CHANNEL_B, TRIANGLE_HEIGHT * ( (double) (-normal_time_ms + TRIANGLE_PERIOD_COUNTS) * FREQ_HZ / SEC_TO_MSEC) ) ;
            }
           
         
            
            //reset the counter every 1000 counts
            
            if (time_ms < FREQ_HZ*SEC_TO_MSEC ) 
            {
                time_ms++; //increment   
            }
            else {time_ms = 0;} //reset every 1kHz
            
            _CP0_SET_COUNT(0);//reset timer
        }
//       
        
        //add a sine function here


        //setVoltage(1,2);
        //setVoltage(0,1);
//        
//      
    }
}