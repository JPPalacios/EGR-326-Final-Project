#include "msp.h"
#include "rtc.h"
#include "stdio.h"
#include "string.h"

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

/* Decodes the date */
char* decode_Date(int day, int month, int date, int year){
    char decDate[100]; 

    sprintf(decDate, "%s, %s %d, %d", decode_Weekday(day), decode_Month(month), date, year);

    return decDate; 
}

/* Decodes the time */
char* decode_Time(int hour, int min, int sec, int ampm){
    char decTime[100]; 

    if(ampm){
        if(min < 10 && sec < 10)
            sprintf(decTime, "%d:0%d:0%d PM", hour, min, sec);
        else if(min < 10 && sec > 10)
            sprintf(decTime, "%d:0%d:%d PM", hour, min, sec);
        else if(min > 10 && sec < 10)
            sprintf(decTime, "%d:%d:0%d PM", hour, min, sec);
        else
            sprintf(decTime, "%d:%d:%d PM", hour, min, sec);
    }else{
        if(min < 10 && sec < 10)
            sprintf(decTime, "%d:0%d:0%d AM", hour, min, sec);
        else if(min < 10 && sec > 10)
            sprintf(decTime, "%d:0%d:%d AM", hour, min, sec);
        else if(min > 10 && sec < 10)
            sprintf(decTime, "%d:%d:0%d AM", hour, min, sec);
        else
            sprintf(decTime, "%d:%d:%d AM", hour, min, sec);
    }
    
    return decTime;
}

/* Decodes the temperature */
char* decode_Temperature(int temperature, int mode){
    char decTemp[100];

    if(mode)
        sprintf(decTemp, "%d F", (int)((1.8 * temperature) + 32));   
    else
        sprintf(decTemp, "%d C", temperature);

    return decTemp;
}

/* Decodes the day of the week */ 
char* decode_Weekday(int day){
    if(day == 1)
        return "Sunday";
    else if(day == 2)
        return "Monday";
    else if(day == 3)
        return "Tuesday";
    else if(day == 4)
        return "Wednesday";
    else if(day == 5)
        return "Thursday";
    else if(day == 6)
        return "Friday";
    else if(day == 7)
        return "Saturday";
    else
        return "Error: Weekday";    
}

/* Decodes month of the year */ 
char* decode_Month(int month){
    if(month == 1)
        return "January";
    else if(month == 2)
        return "February";
    else if(month == 3)
        return "March";
    else if(month == 4)
        return "April";
    else if(month == 5)
        return "May";
    else if(month == 6)
        return "June";
    else if(month == 7)
        return "July";
    else if(month == 8)
        return "August";
    else if(month == 9)
        return "September";
    else if(month == 10)
        return "October";
    else if(month == 11)
        return "November";
    else if(month == 12)
        return "December";
    else 
        return "Error: Month"; 
}
