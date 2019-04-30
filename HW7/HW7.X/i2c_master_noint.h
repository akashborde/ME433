#ifndef I2C_MASTER_NOINT_H__
#define I2C_MASTER_NOINT_H__
// Header file for i2c_master_noint.c
// helps implement use I2C1 as a master without using interrupts


//read/write addresses to the IMU
#define i2c_WRITE_ADDR 0b11010110 //110101 (hardcoded) 1 (SA0 pulled up on breakout) 0 (write)
#define i2c_READ_ADDR 0b11010111 //110101 (hardcoded) 1 (SA0 pulled up on breakout) 1 (read)
#define i2c_WHOAMI_SFR 0x0F //whoami register
#define WHOAMI_ID 0b01101001 

//these settings are from page 46-48 of IMU datasheet
//settings for control register
#define IMU_CTRL1_XL 0x10 
//1.66kHz frequency
#define FREQ_1_66_KHZ   0b10000000 // <7:4> are 1000,rest are 0
//(+/-) 2g sensitivity. 
#define SENS_2G         0b00000000 // <3:2> are 01, rest are 0   
#define FILT_100HZ      0b00000010 // <1:0> are 10, rest are 0


#define IMU_CTRL2_G  0x11
#define SENS_1000DPS 0b00001000 //<3:2> are 10, rest are 0

#define IMU_CTRL3_C  0x12
#define MULT_BYTE_ACCESS 0b00000100 //this is set by default. I don't get why we need to set this

#define OUT_TEMP_L  0x20 //FIRST register to read from sequentailly
#define NUM_READS   14

void i2c_master_setup(void);              // set up I2C 1 as a master, at 400 kHz

void i2c_master_start(void);              // send a START signal
void i2c_master_restart(void);            // send a RESTART signal
void i2c_master_send(unsigned char byte); // send a byte (either an address or data)
unsigned char i2c_master_recv(void);      // receive a byte of data
void i2c_master_ack(int val);             // send an ACK (0) or NACK (1)
void i2c_master_stop(void);               // send a stop

void IMU_Setup(void);				  //initialize the MCP23008
//void setExpander(char pin, char level);	  //write using i2c
char IMU_read(void);						  //read using i2c	

//read multiple registers real quick starting with register r
//feeds into array called data with length len
void IMU_read_multiple(unsigned char r, unsigned char * data, int len);


#endif
