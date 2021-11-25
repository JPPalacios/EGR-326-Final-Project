/*  Title:          EGR 326 Final Project: Automotive Dashboard
    Filename:       main.c
    Engineer(s):    JP Palacios, Megan Healy
    Date:           11/24/2021
    Instructor:     Professor Brian Krug
    Description:  	This program is intended to simulate an automotive dashboard
    Note:																*/

/* Header files: */
#include "msp.h"
#include "stdio.h"
#include "RTC.h"

void main(void){

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    I2C_Setup();

	writeRTC.tSec = 5; writeRTC.oSec = 9;
    writeRTC.tMin = 5; writeRTC.oMin = 9;
	writeRTC.hourMode = 1; writeRTC.ampm = 1;
	writeRTC.tHour  = 1; writeRTC.oHour = 1;
    writeRTC.day    = 7;
	writeRTC.tDate  = 3;  writeRTC.oDate = 1;
    writeRTC.tMonth = 1; writeRTC.oMonth = 2;
    writeRTC.tYear  = 2;  writeRTC.oYear = 1;

	//readRTC;

	temperature_mode = 1;	// Display Ferenheit

	write_RTC();


	while(1){

	    read_RTC();

		printf("Date: ");
		puts(decode_Date(readRTC.day, readRTC.month, readRTC.date, readRTC.year));
		printf("Time: ");
		puts(decode_Time(readRTC.hour, readRTC.min, readRTC.sec, readRTC.ampm) );
		printf("Temperature: ");
		puts(decode_Temperature(readRTC.temperature, temperature_mode));

	}

}

// The code below belongs in main.c
/* Writes time to the RTC */
void write_RTC(){
    I2C_Write(RTC_ADDR, 0x00, ((writeRTC.tSec << 4)     +  writeRTC.oSec) );   // seconds  
    I2C_Write(RTC_ADDR, 0x01, ((writeRTC.tMin << 4)     +  writeRTC.oMin) );   // minutes
    I2C_Write(RTC_ADDR, 0x02, ((writeRTC.hourMode << 6) + (writeRTC.ampm << 5) + (writeRTC.tHour << 4) + writeRTC.oHour) );   // hours
    I2C_Write(RTC_ADDR, 0x03, ((writeRTC.day)));                               // day of the week
    I2C_Write(RTC_ADDR, 0x04, ((writeRTC.tDate << 4)    +  writeRTC.oDate));   // year
    I2C_Write(RTC_ADDR, 0x05, ((writeRTC.century << 7)  + (writeRTC.tMonth << 4) + writeRTC.oMonth));   // century, month
    I2C_Write(RTC_ADDR, 0x06, ((writeRTC.tYear << 4)    +  writeRTC.oYear));   // year  
}

/* Reads time from the RTC */
void read_RTC(void){
    unsigned char data_read; 
	unsigned char data_temp; 
    int error; 

    do{
        error = I2C_Read(RTC_ADDR, 0x00, &data_read);   // Read seconds
        readRTC.tSec = data_read >> 4; 
        readRTC.oSec = data_read & 0x0F;
		readRTC.sec  = (readRTC.tSec * 10) + readRTC.oSec; 		 

        error = I2C_Read(RTC_ADDR, 0x01, &data_read);   // Read minutes
        readRTC.tMin = data_read >> 4; 
        readRTC.oMin = data_read & 0x0F;
		readRTC.min  = (readRTC.tMin * 10) + readRTC.oMin;

        error = I2C_Read(RTC_ADDR, 0x02, &data_read);   // Read hour mode, am/pm, hours
		data_temp = data_read;
		readRTC.hourMode = data_temp >> 6;
		data_temp = data_read;
        readRTC.ampm = (data_temp >> 5) & 0x01; 
		data_read &= 0x1F; 
		readRTC.tHour    = data_read >> 4;
        readRTC.oHour    = data_read & 0x0F;
		readRTC.hour  = (readRTC.tHour * 10) + readRTC.oHour;

        error = I2C_Read(RTC_ADDR, 0x03, &data_read);   // Read weekday
        readRTC.day = data_read; 

        error = I2C_Read(RTC_ADDR, 0x04, &data_read);   // Read minutes
        readRTC.tDate = data_read >> 4; 
        readRTC.oDate = data_read & 0x0F;
		readRTC.date  = (readRTC.tDate * 10) + readRTC.oDate;

        error = I2C_Read(RTC_ADDR, 0x05, &data_read);   // Read century, month
        readRTC.century   = data_read >> 7;
        readRTC.tMonth    = data_read >> 4;
        readRTC.oMonth    = data_read & 0x0F;
		readRTC.month  = (readRTC.tMonth * 10) + readRTC.oMonth;

        error = I2C_Read(RTC_ADDR, 0x06, &data_read);   // Read minutes
        readRTC.tYear = data_read >> 4; 
        readRTC.oYear = data_read & 0x0F;
		readRTC.year  = 2000 + (readRTC.tYear * 10) + readRTC.oYear;

        error = I2C_Read(RTC_ADDR, 0x11, &data_read);   // Read Temperature (Celcius)
        readRTC.temperature = data_read; 

    } while(error);
}

/* TO-DO: Reads time from the EEPROM */
void read_EEP(void){ /*...*/ }

////////////////////////////////		printf("Date: ");
		// puts(decode_Date(readRTC.day, readRTC.month, readRTC.date, readRTC.year));
		// printf("Time: ");
		// puts(decode_Time(readRTC.hour, readRTC.min, readRTC.sec, readRTC.ampm) );
		// printf("Temperature : ");
		// puts(decode_Temperature(readRTC.temperature, temperature_mode));
