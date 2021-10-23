#ifndef I2C_H_
#define I2C_H_

#define SLAVE_ADDR 0x68 //DS1337 1101 0000 RCT slave addr

void I2C_setup();

// Single Byte Mode
int I2C_Write(int slaveAddr, unsigned char memAddr, unsigned char data); 
int I2C_Read(int slaveAddr, unsigned char memAddr, unsigned char* data);

// Multi Byte Mode
int I2C_burstWrite(int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data);
int I2C_burstRead (int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data);

unsigned char data_send[15] = {0x89, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83};
unsigned char data_read[7] = {};

#endif /* I2C_H_ */