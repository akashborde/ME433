// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Change I2C2 to the I2C channel you are using
// I2C pins need pull-up resistors, 2k-10k


#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include "i2c_master_noint.h"

void i2c_master_setup(void) {
  ANSELBbits.ANSB2 = 0; 	//turn off analog input for Pin B2 aka SDA2
  ANSELBbits.ANSB3 = 0; 	//turn off analog input for Pin B3 aka SCL2
  I2C2CONbits.ON = 1;               // turn on the I2C2 module
  I2C2BRG = 90;            //400kHz means 90, according to pg 192 of ME333 text
}

// Start a transmission on the I2C bus
void i2c_master_start(void) {
    I2C2CONbits.SEN = 1;            // send the start bit
    while(I2C2CONbits.SEN) { ; }    // wait for the start bit to be sent
}

void i2c_master_restart(void) {     
    I2C2CONbits.RSEN = 1;           // send a restart 
    while(I2C2CONbits.RSEN) { ; }   // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
  I2C2TRN = byte;                   // if an address, bit 0 = 0 for write, 1 for read
  while(I2C2STATbits.TRSTAT) { ; }  // wait for the transmission to finish
  if(I2C2STATbits.ACKSTAT) {        // if this is high, slave has not acknowledged
    // ("I2C2 Master: failed to receive ACK\r\n");
    //while(1)
    //{;} //hang the program if no acknowledgement received
  }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C2CONbits.RCEN = 1;             // start receiving data
    while(!I2C2STATbits.RBF) { ; }    // wait to receive the data
    return I2C2RCV;                   // read and return the data
}

void i2c_master_ack(int val) {        // sends ACK = 0 (slave should send another byte)
                                      // or NACK = 1 (no more bytes requested from slave)
    I2C2CONbits.ACKDT = val;          // store ACK/NACK in ACKDT
    I2C2CONbits.ACKEN = 1;            // send ACKDT
    while(I2C2CONbits.ACKEN) { ; }    // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) {          // send a STOP:
  I2C2CONbits.PEN = 1;                // comm is complete and master relinquishes bus
  while(I2C2CONbits.PEN) { ; }        // wait for STOP to complete
}

void IMU_Setup(void)
{
	i2c_master_setup(); //setup SPI
    
    //send message to IMU about accelerometer
    i2c_master_start(); //start signal for writing
    i2c_master_send(i2c_WRITE_ADDR);
    //write to the CTRL1_XL register
    i2c_master_send(IMU_CTRL1_XL);
    //set the frequency, sensitivity (g) and filter
    i2c_master_send(FREQ_1_66_KHZ | SENS_2G | FILT_100HZ);    
    //stop
    i2c_master_stop(); //stop signal
    
    i2c_master_start(); //start signal for writing
    i2c_master_send(i2c_WRITE_ADDR);
    //write to gyroscope register
    i2c_master_send(IMU_CTRL2_G);
    //set frequency and sensitivity
    i2c_master_send(FREQ_1_66_KHZ | SENS_1000DPS);
    //STOP
    i2c_master_stop();
    
    i2c_master_start(); //start signal for writing
    i2c_master_send(i2c_WRITE_ADDR);
    //write to cntl register
    i2c_master_send(IMU_CTRL3_C);
    //set multiple byte access command
    i2c_master_send(MULT_BYTE_ACCESS);
    //STOP
    i2c_master_stop();
    
}

//void setExpander(char pin, char level)
//{
//    i2c_master_start(); // Begin the start sequence
//	//define where to send the i2c message
//    i2c_master_send(i2c_WRITE_ADDR); //send addr with a 0 for write
////    //define what SFR on the i2c chip to write to
//    i2c_master_send(i2c_WHOAMI_SFR);
//    // send the actual data
//    //i2c_master_send(level << pin);
//    //stop
//    i2c_master_stop();
//}

char IMU_read()
{
    char read;
    //begin start sequence of a write
    i2c_master_start();
    //defining the MCP expander chip as the target of this read
    i2c_master_send(i2c_WRITE_ADDR);       // pic address and read byte
    //define which SFR you're gonna write to        
    i2c_master_send(i2c_WHOAMI_SFR);         // 
    i2c_master_restart();                   // send a RESTART so we can begin reading 
    i2c_master_send(i2c_READ_ADDR); // send slave address, left shifted by 1,
                                            // and then a 1 in lsb, indicating read
    read = i2c_master_recv();       // receive a byte from the bus
//        i2c_master_ack(0);                      // send ACK (0): master wants another byte!
//        master_read1 = i2c_master_recv();       // receive another byte from the bus
    i2c_master_ack(1);                      // send NACK (1):  master needs no more bytes
    i2c_master_stop();                      // send STOP:  end transmission, give
    return read;
}

void IMU_read_multiple(unsigned char r, unsigned char * data, int length)
{
    /*
     * Reads sequentially from the IMU
     * @param:  r    first register to read
     * @param   *data       pointer to array to store data
     * @param:  length      length of array     * 
     */
    
        
    i2c_master_start(); //start the read
    // pic address and read byte
    i2c_master_send(i2c_WRITE_ADDR);       
    //send the first register to read from (should be OUT_TEMP_L)
    i2c_master_send(r); 
    i2c_master_restart();  // send a RESTART so we can begin reading
    i2c_master_send(i2c_READ_ADDR); // send slave address, with a 1 indicating read
    
    int i; //for loop index
    for (i = 0; i < length; i++)
    {
       data[i] = i2c_master_recv(); //read a byte, put it in the array

       if (i < (length - 1)) //for every time except the last
       {
           i2c_master_ack(0); //keep reading
       }
       else //for the last time 
       {
           i2c_master_ack(1); //stop reading
       }                                     
    }    
}



