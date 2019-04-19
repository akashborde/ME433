#include <math.h> 	//for sine wave plotting
#include "spi.h"
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>

void initSPI1()
{
         
    SPI1CON = 0; // turn off SPI module and reset it
    SPI1BUF; //clear the rx buffer by reading from it  
    SPI1BRG = 1; // according to the reference manual, this is the fastest clk
    /*
     * Ref manual:  61106G
     * Page:        29
     * F_sck = F_pb / ( 2 * (SPIBRG + 1))
     * But in HW4, nick mentioned SPI1BRG >= 1
     */
    
    //manual suggested clearing ENHBUF
    SPI1CONbits.ENHBUF = 0;    
    SPI1STATbits.SPIROV = 0; //clear the overflow bit
    SPI1CONbits.MSTEN = 1; // enable master mode
    //Chose RPB13 for SDO1 -> set RPB13R<3:0> = 0011 (pg 133). Pin 24
    RPB13Rbits.RPB13R = 0b0011;
    //Chose Pin 26 for nCS
    TRISBbits.TRISB15 = 0; //set this as an output
    //LATBbits.LATB15 will be the control for this output
    
    SPI1CONbits.ON = 1; //turn SPI1 on
    /*
     * MCP4922 data comes in on rising edge of clock
     * Default setup for PIC SPI clock is idle = low. SPI1CONbits.CKP = 0;
     * Default setup for PIC SPI clock read is low to high edge <- data
     * :: Don't need to modify this part.
     *   
     */
  
}

unsigned char SPI1_IO(unsigned char write)
{
    //unsigned char is a byte, so this works nicely
    SPI1BUF = write;
    while(!SPI1STATbits.SPIRBF) //automatically set when transmit/receive succeeds
    { ; }
    // make something that writes chars
    return SPI1BUF;      // wait to receive the byte

}


void setVoltage(char a, double v) 
{
    //set up the spi settings, specifically the channel   
    unsigned char spi_settings; 
    unsigned char channel_select = a << 7; // move to the first bit <15>
    // bits <14:12>
    //bit 14: Input Buffer. 1 for buffered
    //bit 13: Gain select. 1 for 1x, 0 for 2x
    //bit 12: Shutdown control. 1 for active, 0 for shutdown
    spi_settings = 0b01110000 | channel_select;
    
    //convert voltage to binary
    /*
     * Input voltage is from 0 to MAX_VOLTAGE (3.3)
     * Output should be from 0 to RESOLUTION (0-4095)
     * Sample calculation: Assuming input is 2v
     * (2v/3.3v) = (output / 4095)
     * output = 4095 * input / 3.3v
     */
    
    double max_voltage = 3.3;
    int max_resolution = 4095;
    double ratio = (double) v / max_voltage;
    int output = max_resolution * ratio; 
    //will be truncated to an integer from 0 to max_resolution
    
    //now, we need to put this into two writes. output will be a 16 bit num from
    //1111 1111 1111 to 0000 0000 0000
    
    char first_four_bytes = output >> 8; //should move the <11:8> bits to the LSB
    first_four_bytes &= 0b00001111; //clear <15:12>
        
    //this combines <15:12> with <11:8>
    char first_write = spi_settings | first_four_bytes;
    char second_write = output & 0x00FF;
    
    //write spi
    
    LATBbits.LATB15 = 0;
    
    SPI1_IO(first_write);
    SPI1_IO(second_write);
    
    LATBbits.LATB15 = 1;
}
   
/*
 * Core Timer: 24MHz, 0.00004166 ms
 * Want a 1kHz frequency for the wave (1ms)
 * Every 24000 counts is 1ms
 */

