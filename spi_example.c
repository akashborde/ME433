//
#define CS LATBbits.LATB8       // chip select pin

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
 SPI4BUF = o;
 while(!SPI4STATbits.SPIRBF) { // wait to receive the byte
   ;
 }
 return SPI4BUF;
}

// initialize spi4 and the ram module
void ram_init() {
 // set up the chip select pin as an output
 // the chip select pin is used by the sram to indicate
 // when a command is beginning (clear CS to low) and when it
 // is ending (set CS high)
 TRISBbits.TRISB8 = 0;
 CS = 1;

 // Master - SPI4, pins are: SDI4(F4), SDO4(F5), SCK4(F13).  
 // we manually control SS4 as a digital output (F12)
 // since the pic is just starting, we know that spi is off. We rely on defaults here

 // setup spi1 , all bits must be changed for SPI1 from SPI4
 SPI4CON = 0;              // turn off the spi module and reset it
 SPI4BUF;                  // clear the rx buffer by reading from it
 SPI4BRG = 0x3;            // baud rate to 10 MHz [SPI4BRG = (80000000/(2*desired))-1]
 SPI4STATbits.SPIROV = 0;  // clear the overflow bit
 SPI4CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
 SPI4CONbits.MSTEN = 1;    // master operation
 SPI4CONbits.ON = 1;       // turn on spi 4

                           // send a ram set status command.
 CS = 0;                   // enable the ram
 spi_io(0x01);             // ram write status
 spi_io(0x41);             // sequential mode (mode = 0b01), hold disabled (hold = 0)
 CS = 1;                   // finish the command
}

// write len bytes to the ram, starting at the address addr
void ram_write(unsigned short addr, const char data[], int len) {
 int i = 0;
 CS = 0;                        // enable the ram by lowering the chip select line
 spi_io(0x2);                   // sequential write operation
 spi_io((addr & 0xFF00) >> 8 ); // most significant byte of address
 spi_io(addr & 0x00FF);         // the least significant address byte
 for(i = 0; i < len; ++i) {
   spi_io(data[i]);
 }
 CS = 1;                        // raise the chip select line, ending communication
}

read len bytes from ram, starting at the address addr
void ram_read(unsigned short addr, char data[], int len) {
 int i = 0;
 CS = 0;
 spi_io(0x3);                   // ram read operation
 spi_io((addr & 0xFF00) >> 8);  // most significant address byte
 spi_io(addr & 0x00FF);         // least significant address byte
 for(i = 0; i < len; ++i) {
   data[i] = spi_io(0);         // read in the data
 }
 CS = 1;
}

int main(void) {
 unsigned short addr1 = 0x1234;                  // the address for writing the ram
 char data[] = "Help, I'm stuck in the RAM!";    // the test message
 char read[] = "***************************";    // buffer for reading from ram
 char buf[100];                                  // buffer for comm. with the user
 unsigned char status;                           // used to verify we set the status 
 NU32_Startup();   // cache on, interrupts on, LED/button init, UART init
 ram_init(); 

 // check the ram status
 CS = 0;
 spi_io(0x5);                                      // ram read status command
 status = spi_io(0);                               // the actual status
 CS = 1;

 sprintf(buf, "Status 0x%x\r\n",status);
 NU32_WriteUART3(buf);

 sprintf(buf,"Writing \"%s\" to ram at address 0x%x\r\n", data, addr1);
 NU32_WriteUART3(buf);
                                                   // write the data to the ram
 ram_write(addr1, data, strlen(data) + 1);         // +1, to send the '\0' character
 ram_read(addr1, read, strlen(data) + 1);          // read the data back
 sprintf(buf,"Read \"%s\" from ram at address 0x%x\r\n", read, addr1);
 NU32_WriteUART3(buf);


	init_spi();

 while(1) {
	_CPO_SET_COUNT(0);
	float f = 512 +512*sin(i*2*3.1415/1000*10);  //should make a 10Hz sin wave)
	i++;



	setVoltage(0,512);		//test
	setVoltage(1,256);		//test

	while(_CPO_GET_COUNT() < 2400000000/1000) {}  //check this is 24Million
   ;
 }
 return 0;
}


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