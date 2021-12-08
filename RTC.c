#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msp.h"
#include "rtc.h"
#include "menu.h"
#include "ST7735.h"



void I2C_Setup(void){
    EUSCI_B1->CTLW0 |= 1;       // disable UCB1 during config
    EUSCI_B1->CTLW0 = 0x0F81;    // 7-bit slave addr, master, I2C, synch mode, use SMCLK
    EUSCI_B1->BRW = 0x18;       // 3MHz/30 = 100kHz, 0x18 = 30
    RTC->SEL0 |=  (SDA | SCL);  // UCB1
    RTC->SEL1 &=~ (SDA | SCL);
    EUSCI_B1->CTLW0 &= ~1;      // enable UCB1 after config
}

/* Writes byte to the receiver address, returns 0 if no errors */  
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

/* Reads byte from the receiver address, returns 0 if no errors */ 
int I2C_Read (int slaveAddr, unsigned char memAddr, unsigned char* data){
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

/* Writes n bytes to the receiver address, returns 0 if no errors */ 
int I2C_BurstWrite(int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data){

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
    EUSCI_B1->CTLW0 |= 0x0004;   //stop bit
    while(EUSCI_B1->CTLW0 & 4);  //wait for stop bit to send
    return 0;                    //return to main if no errors
}

/* Reads n bytes to the receiver address, returns 0 if no errors */ 
int I2C_BurstRead (int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data){

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
    return 0;                   //return to main if no errors
}

/* Decodes the day of the week */ 
char* decode_Weekday(int day){

    // char lut_weekday[8][3] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Err"};
    // return lut_weekday[day - 1];
    // if(day == 1)
    //     return "Sunday";
    // else if(day == 2)
    //     return "Monday";
    // else if(day == 3)
    //     return "Tuesday";
    // else if(day == 4)
    //     return "Wednesday";
    // else if(day == 5)
    //     return "Thursday";
    // else if(day == 6)
    //     return "Friday";
    // else if(day == 7)
    //     return "Saturday";
    // else
    //     return "Error";  

    if(day == 1)
        return "Sun";
    else if(day == 2)
        return "Mon";
    else if(day == 3)
        return "Tue";
    else if(day == 4)
        return "Wed";
    else if(day == 5)
        return "Thu";
    else if(day == 6)
        return "Fri";
    else if(day == 7)
        return "Sat";
    else
        return "Err";    
}

/* Decodes month of the year */ 
char* decode_Month(int month){
//    char lut_month[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "Err"};
//    return lut_month[month - 1];
    //  if(month == 1)
    //      return "January";
    //  else if(month == 2)
    //      return "February";
    //  else if(month == 3)
    //      return "March";
    //  else if(month == 4)
    //      return "April";
    //  else if(month == 5)
    //      return "May";
    //  else if(month == 6)
    //      return "June";
    //  else if(month == 7)
    //      return "July";
    //  else if(month == 8)
    //      return "August";
    //  else if(month == 9)
    //      return "September";
    //  else if(month == 10)
    //      return "October";
    //  else if(month == 11)
    //      return "November";
    //  else if(month == 12)
    //      return "December";
    //  else
    //      return "Error";

     if(month == 1)
         return "Jan";
     else if(month == 2)
         return "Feb";
     else if(month == 3)
         return "Mar";
     else if(month == 4)
         return "Apr";
     else if(month == 5)
         return "May";
     else if(month == 6)
         return "Jun";
     else if(month == 7)
         return "Jul";
     else if(month == 8)
         return "Aug";
     else if(month == 9)
         return "Sepr";
     else if(month == 10)
         return "Oct";
     else if(month == 11)
         return "Nov";
     else if(month == 12)
         return "Dec";
     else
         return "Err";
}
