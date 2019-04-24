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
    // Pushbutton pin: B4
    TRISBbits.TRISB4 = 1; // Set it as an input
        
    // Green LED pin:  A4
    TRISAbits.TRISA4 = 0; //set as output
    LATAbits.LATA4 = 1;   //initialized as high

    //i2c_master_setup(); //initialize i2c2 register
    initExpander(); // initialize the i2c

    __builtin_enable_interrupts();
    
     i2c_master_start(); // Begin the start sequence
    //define where to send the i2c message
    i2c_master_send(i2c_WRITE_ADDR); //send addr with a 0 for write
    //define what SFR on the i2c chip to write to
    i2c_master_send(i2c_IODIR_SFR);
    //send the value to go to the SFR
    //set 0-3 output and 4-7 input
    i2c_master_send( GPIO_0to3_OUTPUT | GPIO_4to7_INPUT);
    //end this thread real quick
    i2c_master_stop();
    
    i2c_master_start(); // Begin the start sequence
    //define where to send the i2c message
    i2c_master_send(i2c_WRITE_ADDR); //send addr with a 0 for write
    //define what SFR on the i2c chip to write to
    i2c_master_send(i2c_GPPU_SFR);
    //send the value to go to the SFR
    //set 0-3 output and 4-7 input
    i2c_master_send(0b1000000); //set bit 1 as pullup (Pin 7)
    //end this thread real quick
    i2c_master_stop();

        
     //_CP0_SET_COUNT(0); //begin timer



    while(1) 
    {
        char master_read0 = 0;//, master_read1;
        
        
        i2c_master_start();                     // Begin the start sequence
        //defining the MCP expander chip as the target of this read
        i2c_master_send(i2c_WRITE_ADDR);       // pic address and read byte
        //define which SFR you're gonna write to        
        i2c_master_send(i2c_GPIO_SFR);         // 
        //define that we want to read the GP7 pin
        i2c_master_send(0b1000000);         // send another byte to the slave
        i2c_master_restart();                   // send a RESTART so we can begin reading 
        i2c_master_send(i2c_READ_ADDR); // send slave address, left shifted by 1,
                                                // and then a 1 in lsb, indicating read
        master_read0 = i2c_master_recv();       // receive a byte from the bus
//        i2c_master_ack(0);                      // send ACK (0): master wants another byte!
//        master_read1 = i2c_master_recv();       // receive another byte from the bus
        i2c_master_ack(1);                      // send NACK (1):  master needs no more bytes
        i2c_master_stop();                      // send STOP:  end transmission, give
       
        //char GP7_status = (master_read0 >> 7); //extract the first bit (GP7 info)
//                
        
        if (master_read0 > 0)//read this input pin
        {
            LATAbits.LATA4 = 0;  //turn off the LED
            //turn RED LED On
             i2c_master_start(); // Begin the start sequence
            //define where to send the i2c message
            i2c_master_send(i2c_WRITE_ADDR); //send addr with a 0 for write
            //define what SFR on the i2c chip to write to
            i2c_master_send(i2c_GPIO_SFR);
            //set the 0th pin to high to turn on the led
            i2c_master_send(0x01); //bit 0 set to high
            i2c_master_stop();
        }
        else
        {        
            LATAbits.LATA4 = 1;   //turn LED on
              //turn RED LED OFF
             i2c_master_start(); // Begin the start sequence
            //define where to send the i2c message
            i2c_master_send(i2c_WRITE_ADDR); //send addr with a 0 for write
            //define what SFR on the i2c chip to write to
            i2c_master_send(i2c_GPIO_SFR);
            //set the 0th pin to high to turn on the led
            i2c_master_send(0x00); //bit 0 set to high
            i2c_master_stop();
        }     
    
    }
}