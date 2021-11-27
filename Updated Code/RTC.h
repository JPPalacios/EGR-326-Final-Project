#ifndef RTC_H_
#define RTC_H_

#define RTC_ADDR 0x68
#define EEP_ADDR 0x50

#define RTC P6
#define SCL BIT5
#define SDA BIT4

typedef struct{
    int tSec, oSec,              // 00h: data_read_RTC >> 4, data_read_RTC & 0x0F
        tMin, oMin,              // 01h
        hourMode, ampm,          // 02h: hour_mode = 1,0 -> 12,24 hour, am_pm = 1,0 -> am, pm
        tHour, oHour,            // 02h
        day,                     // 03h
        tDate, oDate,            // 04h
        century, tMonth, oMonth, // 05h
        tYear, oYear;            // 06h
    // alarm stuff + control     // 07h -> 0Fh
    signed int temperature;      // 11h
    int temperature_mode; 

    int sec, min, hour, date, month, year;  // Printer-friendly time format
} time;

time writeRTC;  // Write time to RTC
time readRTC;   // Read time from RTC
time readEEP;   // Read time from EEP

volatile int temperature_mode;  // 0, 1 -> celcius, ferenheit  

void I2C_Setup(void);
int  I2C_Write(int slaveAddr, unsigned char memAddr, unsigned char data);    // Single bytewrite and read
int  I2C_Read (int slaveAddr, unsigned char memAddr, unsigned char* data);
int  I2C_BurstWrite(int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data);  // Multi byte write and read
int  I2C_BurstRead (int slaveAdd, unsigned char memAdd, int numBytes, unsigned char* data);

void read_RTC(void);    // Struct function will be in main
void write_RTC(void); 

//char* decode_Time(char* decTime);

// char* decode_RTC();
// char* decode_Date();

//char* decode_Temperature(int temperature, int mode);
char* decode_Weekday(int day);
char* decode_Month  (int month); 

#endif /* RTC_H_ */
