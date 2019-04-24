#ifndef I2C_MASTER_NOINT_H__
#define I2C_MASTER_NOINT_H__
// Header file for i2c_master_noint.c
// helps implement use I2C1 as a master without using interrupts

#define i2c_WRITE_ADDR 0b01000000 //0100(datasheet),000(<A2:A0> GND),0(write)
#define i2c_READ_ADDR 0b01000001 //0100(datasheet),000(<A2:A0> GND),1(read)
#define i2c_IODIR_SFR 0x00 //IODIR 
#define i2c_GPIO_SFR 0x09 //GPIO
#define i2c_OLAT_SFR 0x0A //OLAT
#define i2c_GPPU_SFR 0x06 //pullup resistor enable
#define GPIO_0to3_OUTPUT 0b00000000
#define GPIO_4to7_INPUT  0b11110000


void i2c_master_setup(void);              // set up I2C 1 as a master, at 400 kHz

void i2c_master_start(void);              // send a START signal
void i2c_master_restart(void);            // send a RESTART signal
void i2c_master_send(unsigned char byte); // send a byte (either an address or data)
unsigned char i2c_master_recv(void);      // receive a byte of data
void i2c_master_ack(int val);             // send an ACK (0) or NACK (1)
void i2c_master_stop(void);               // send a stop

void initExpander(void);				  //initialize the MCP23008
//void setExpander(char pin, char level);	  //write using i2c
//char getExpander(void);						  //read using i2c	

#endif
