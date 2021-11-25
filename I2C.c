#include "msp.h"

#define RTC P6
#define SCL BIT5
#define SDA BIT4

//unsigned char data_send[15] = {0x89, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83};
//unsigned char data_read[7] = {};

void I2C_setup(){

    ///MASTER TRANSMIT INITIALIZATION - UCBI as I2C:
    RTC->SEL0        |= (SDA | SCL); //BIT4 - SDA, BIT5 - SCL
    RTC->SEL1        &= ~(SDA | SCL); //BIT4 - SDA, BIT5 - SCL

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;
    EUSCI_B1->CTLW0 = 0x0f81;
    EUSCI_B1->BRW    = 30;
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
}

// Single Byte Mode
int I2C_Write(int slaveAddr, unsigned char memAddr, unsigned char data){

    EUSCI_B1->I2CSA = slaveAddr;    /* setup slave address */
    EUSCI_B1->CTLW0 |= 0x0010;      /* enable transmitter */
    EUSCI_B1->CTLW0 |= 0x0002;      /* generate START and send slave address */
    while((EUSCI_B1->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B1->TXBUF = memAddr;      /* send memory address to slave */
    while(!(EUSCI_B1->IFG & 2));    /* wait till it's ready to transmit */
    EUSCI_B1->TXBUF = data;         /* send data to slave */
    while(!(EUSCI_B1->IFG & 2));    /* wait till last transmit is done */
    EUSCI_B1->CTLW0 |= 0x0004;      /* send STOP */
    while(EUSCI_B1->CTLW0 & 4) ;    /* wait until STOP is sent */
    return 0;                       /* no error */
}

int I2C_Read(int slaveAddr, unsigned char memAddr, unsigned char* data){
    EUSCI_B1->I2CSA = slaveAddr;    /* setup slave address */
    EUSCI_B1->CTLW0 |= 0x0010;      /* enable transmitter */
    EUSCI_B1->CTLW0 |= 0x0002;      /* generate START and send slave address */
    while((EUSCI_B1->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B1->TXBUF = memAddr;      /* send memory address to slave */
    while(!(EUSCI_B1->IFG & 2));    /* wait till it's ready to transmit */
    EUSCI_B1->CTLW0 &= ~0x0010;     /* enable receiver */
    EUSCI_B1->CTLW0 |= 0x0002;      /* generate RESTART and send slave address */
    while(EUSCI_B1->CTLW0 & 2);     /* wait till restart is finished */
    EUSCI_B1->CTLW0 |= 0x0004;      /* setup to send STOP after the byte is received */
    while(!(EUSCI_B1->IFG & 1));    /* wait till data is received */
    *data = EUSCI_B1->RXBUF;        /* read the received data */
    while(EUSCI_B1->CTLW0 & 4) ;    /* wait until STOP is sent */
    return 0;                       /* no error */
}

// Multi Byte Mode
int I2C_burstWrite(int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data){

    if(numBytes <= 0)
        return -1; //invalid write

    EUSCI_B1->I2CSA = slaveAdd; //set up slave address
    EUSCI_B1->CTLW0 |= 0x0010; //enable transmitter
    EUSCI_B1->CTLW0 |= 0x0002; //transmit buffer is brake or brake sync
    while(!(EUSCI_B1->IFG & 2)); //wait until master ready
    EUSCI_B1->TXBUF = memAdd; //send memory address to slave

    //send data 1 byte at a time
    do{
        while(!(EUSCI_B1->IFG & 2));  //wait to transmit until ready
        EUSCI_B1->TXBUF = *data++;  //data to slave
        numBytes--;
    } while(numBytes > 0);

    while(!(EUSCI_B1->IFG & 2)); //wait until master ready
    EUSCI_B1->CTLW0 |= 0x0004; //stop bit
    while(EUSCI_B1->CTLW0 & 4); //wait for stop bit to send

    return 0; //return to main if no errors
}

int I2C_burstRead(int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data){

    if (numBytes <= 0)
        return -1;              /* no read was performed */

    EUSCI_B1->I2CSA = slaveAdd; //init slave address
    EUSCI_B1->CTLW0 |= 0x0010; //enable transmit
    EUSCI_B1->CTLW0 |= 0x0002; //generate start bit/send slave address
    while(EUSCI_B1->CTLW0 & 2); //wait for slave address to send
    EUSCI_B1->TXBUF = memAdd; //send memory address to slave
    while(!(EUSCI_B1->IFG & 2)); //wait for transmit to complete
    EUSCI_B1->CTLW0 &= ~0x0010; //enable receiver
    EUSCI_B1->CTLW0 |= 0x0002; //generate restart bit/send slave address
    while(EUSCI_B1->CTLW0 & 2); //wait for restart to complete

    do { //to receive data one byte at a time:
        if (numBytes == 1){ //when only one byte of data is left
            EUSCI_B1->CTLW0 |= 0x0004; //send stop bit after last byte received
        }
        while(!(EUSCI_B1->IFG & 1)); //wait for data to be received
        *data++ = EUSCI_B1->RXBUF; //read data in
        numBytes--;
    } while(numBytes); //until count reaches 0

    while(EUSCI_B1->CTLW0 & 4); //wait for stop bit

    return 0; //return to main if no errors
}

