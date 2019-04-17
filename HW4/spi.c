//#include "NU32.h"       // constants, funcs for startup and UART
#include <math.h> 	//for sine wave plotting
#include "spi.h"
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>

// Demonstrates spi by accessing external ram
// PIC is the master, ram is the slave
// Uses microchip 23K256 ram chip (see the data sheet for protocol details)
// SDO4 -> SI (pin F5 -> pin 5)
// SDI4 -> SO (pin F4 -> pin 2)
// SCK4 -> SCK (pin B14 -> pin 6)
// SS4 -> CS (pin B8 -> pin 1)
// Additional SRAM connections
// Vss (Pin 4) -> ground
// Vcc (Pin 8) -> 3.3 V
// Hold (pin 7) -> 3.3 V (we don't use the hold function)
// 
// Only uses the SRAM's sequential mode
//

void initSPI1()
{
    //__builtin_disable_interrupts(); //maybe don't need this if its in the main method
    
    //WHAT ARE THE PERIPHERAL PIN SELECT THINGS
    //DO I JUST DO TRIS AND SET A RANDOM PIN AS CHIP SELECT WITH LAT
    //I DONT KNOW WHAT TO DO
    //PLEASE HELP

    //after that this initialization function is complete
        
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
     //__builtin_enable_interrupts(); //maybe don't need this if its in the main method
        
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

/*
unsigned char spi_io(unsigned char o) 
{
     SPI1BUF = o;
     while(!SPI1STATbits.SPIRBF) 
       ;
     }
     return SPI4BUF;
}
*/


void setVoltage(char a, int v) 
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
    
    
    
    
    
    // the actual write is pretty easy
    // 

    // unsigned short t = 0;
    // t= a << 15; //a is at the very end of the data transfer
    // t = t | 0b01110000000000000;
    // t = t | ((v&0b1111111111) <<2); //rejecting excessive bits (above 10)
    
    // CS = 0;
    // spi_io(t>>8);
}

//
//#define CS LATBbits.LATB8       // chip select pin
//
//// send a byte via spi and return the response
//unsigned char spi_io(unsigned char o) {
//  SPI4BUF = o;
//  while(!SPI4STATbits.SPIRBF) { // wait to receive the byte
//    ;
//  }
//  return SPI4BUF;
//}
//
//// initialize spi4 and the ram module
//void ram_init() {
//  // set up the chip select pin as an output
//  // the chip select pin is used by the sram to indicate
//  // when a command is beginning (clear CS to low) and when it
//  // is ending (set CS high)
//  TRISBbits.TRISB8 = 0;
//  CS = 1;
//
//  // Master - SPI4, pins are: SDI4(F4), SDO4(F5), SCK4(F13).  
//  // we manually control SS4 as a digital output (F12)
//  // since the pic is just starting, we know that spi is off. We rely on defaults here
// 
//  // setup spi1 , all bits must be changed for SPI1 from SPI4
//  SPI4CON = 0;              // turn off the spi module and reset it
//  SPI4BUF;                  // clear the rx buffer by reading from it
//  SPI4BRG = 0x3;            // baud rate to 10 MHz [SPI4BRG = (80000000/(2*desired))-1]
//  SPI4STATbits.SPIROV = 0;  // clear the overflow bit
//  SPI4CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
//  SPI4CONbits.MSTEN = 1;    // master operation
//  SPI4CONbits.ON = 1;       // turn on spi 4
//
//                            // send a ram set status command.
//  CS = 0;                   // enable the ram
//  spi_io(0x01);             // ram write status
//  spi_io(0x41);             // sequential mode (mode = 0b01), hold disabled (hold = 0)
//  CS = 1;                   // finish the command
//}
//
//// write len bytes to the ram, starting at the address addr
//void ram_write(unsigned short addr, const char data[], int len) {
//  int i = 0;
//  CS = 0;                        // enable the ram by lowering the chip select line
//  spi_io(0x2);                   // sequential write operation
//  spi_io((addr & 0xFF00) >> 8 ); // most significant byte of address
//  spi_io(addr & 0x00FF);         // the least significant address byte
//  for(i = 0; i < len; ++i) {
//    spi_io(data[i]);
//  }
//  CS = 1;                        // raise the chip select line, ending communication
//}
//
//// read len bytes from ram, starting at the address addr
//void ram_read(unsigned short addr, char data[], int len) {
//  int i = 0;
//  CS = 0;
//  spi_io(0x3);                   // ram read operation
//  spi_io((addr & 0xFF00) >> 8);  // most significant address byte
//  spi_io(addr & 0x00FF);         // least significant address byte
//  for(i = 0; i < len; ++i) {
//    data[i] = spi_io(0);         // read in the data
//  }
//  CS = 1;
//}
//
//int main(void) {
//  unsigned short addr1 = 0x1234;                  // the address for writing the ram
//  char data[] = "Help, I'm stuck in the RAM!";    // the test message
//  char read[] = "***************************";    // buffer for reading from ram
//  char buf[100];                                  // buffer for comm. with the user
//  unsigned char status;                           // used to verify we set the status 
//  NU32_Startup();   // cache on, interrupts on, LED/button init, UART init
//  ram_init(); 
//
//  // check the ram status
//  CS = 0;
//  spi_io(0x5);                                      // ram read status command
//  status = spi_io(0);                               // the actual status
//  CS = 1;
//
//  sprintf(buf, "Status 0x%x\r\n",status);
//  NU32_WriteUART3(buf);
//
//  sprintf(buf,"Writing \"%s\" to ram at address 0x%x\r\n", data, addr1);
//  NU32_WriteUART3(buf);
//                                                    // write the data to the ram
//  ram_write(addr1, data, strlen(data) + 1);         // +1, to send the '\0' character
//  ram_read(addr1, read, strlen(data) + 1);          // read the data back
//  sprintf(buf,"Read \"%s\" from ram at address 0x%x\r\n", read, addr1);
//  NU32_WriteUART3(buf);
//
//
//	init_spi();
//
//  while(1) {
//	_CPO_SET_COUNT(0);
//	float f = 512 +512*sin(i*2*3.1415/1000*10);  //should make a 10Hz sin wave)
//	i++;
//
//
//
//	setVoltage(0,512);		//test
//	setVoltage(1,256);		//test
//
//	while(_CPO_GET_COUNT() < 2400000000/1000) {}  //check this is 24Million
//    ;
//  }
//  return 0;
//}


/*
void setVoltage(char a, int v) {

	unsigned short t = 0;
	t= a << 15; //a is at the very end of the data transfer
	t = t | 0b01110000000000000;
	t = t | ((v&0b1111111111) <<2); //rejecting excessive bits (above 10)
	
	CS = 0;
	spi_io(t>>8);
	spi_
 * }
 */	
