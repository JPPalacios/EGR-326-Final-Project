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
#include "string.h"
#include "stdbool.h"

/* Custom Libraries */ 
#include "main.h"
#include "RTC.h"
#include "encoder.h"
#include "ST7735.h"
#include "menu.h"
#include "Proximity.h"
#include "Timers.h"
#include "stepper.h"

/********** TURN SIGNAL DEFINITIONS **********/
#define TURN_SIG   P3
#define TURN_BLINK BIT3
#define TURN_SIG_PERIOD 3000 // REMOVE
#define TURN_SIG_HALFSEC 500

void turnSignal_Setup(void);
void turn_Signals(void); 
/*********************************************/

/********** TIMER DEFINITIONS ****************/
extern uint8_t SysTick_count;
/*********************************************/


/********** MOTOR DEFINITIONS ****************/
//volatile int count = 0;
//extern volatile uint8_t direction = 0;
//extern volatile int steps;
//extern volatile int hall_flag = 0;
//extern volatile int countVar = 0;
//extern volatile float cur[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
//extern volatile float averageRPS = 0.0, rollingSum = 0.0;
//extern volatile float averageRPM = 0.0;
//extern volatile float averageMPH = 0.0;
//extern int currSteps = 0;
//extern int prevSteps = 0;
//extern int tachoSteps = 0;


//extern volatile int hall_flag;
//extern volatile int countVar;
//extern volatile float cur[5];
//extern volatile float averageRPS, rollingSum;
//extern volatile float averageRPM;
//extern volatile float averageMPH;
//extern volatile int currSteps;
//extern volatile int prevSteps;
//extern volatile int tachoSteps;
/*********************************************/

#define TESTING_SCREEN Splash

extern bool swFlag;
extern bool cwFlag, ccwFlag;
extern volatile int rotaryPos;
volatile bool refresh = false;  
volatile bool printLCDOnce = true; 
volatile bool highlightRefresh = true; 


void main(void){

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // Timer Setup 
	SysTick_Setup();
	Timer32_Setup();

    // Turn Signal Setup
    turnSignal_Setup();

    // Proximity Sensor Setup
    TimerA3_Setup();    // 2 modules, echo + LED PWM
    TimerA0_Setup();    // 1 module trig

    // Motor Setup
    hall_Setup();
    stepper_Setup();

    // Menu Setup
    I2C_Setup();
	encoder_Setup();
    Clock_Init48MHz();
    
	writeRTC.tSec = 5; writeRTC.oSec = 5;
    writeRTC.tMin = 5; writeRTC.oMin = 9;
	writeRTC.hourMode = 1; writeRTC.ampm = 1;
	writeRTC.tHour  = 1; writeRTC.oHour = 2;

    writeRTC.day    = 7;
	writeRTC.tDate  = 3;  writeRTC.oDate = 1;
    writeRTC.tMonth = 1; writeRTC.oMonth = 2;
    writeRTC.tYear  = 2;  writeRTC.oYear = 1;
	writeRTC.temperature_mode = 1;
	 
	write_RTC();
	ST7735_InitR(INITR_REDTAB);
	//ST7735_FillScreen(ST7735_BLACK);
	Output_Clear();

	menu_choice screen = TESTING_SCREEN;
//    tempHeldState  = Step1;
//    speedHeldState = Step1;

//    count = 1;
//    direction = 0;
//    steps = MAXT;

    WDT_Reset();

	while(1){
	    run_Tachometer((int)averageRPM);

        turn_Signals();
        read_Proximity();

        /* Update the RTC, LCD*/
	    read_RTC();

		menu_Setup(screen);

        if(refresh){
            refresh = false;
            ST7735_FillScreen(ST7735_BLACK);
        }

		switch(screen){
			case Splash:

                // FIXME: Add splash screen image
				LCD_header(splashScreen);
				ST7735_DrawFastHLine(0, 12, 130, ST7735_WHITE);

				LCD_body(splashScreen, rotaryPos);

				ST7735_DrawFastHLine(0, 145, 130, ST7735_WHITE);
                LCD_footer(splashScreen);

                if(cwFlag){                 // Clockwise selection
                    cwFlag = false;
                    rotaryPos++;

                    if(rotaryPos < 2)
                        rotaryPos = 0;

                    LCD_highlight(Splash, splashScreen, rotaryPos);

                }else if(ccwFlag){          // Counter-Clockwise selection
                    ccwFlag = false;
                    rotaryPos--;

                    if(rotaryPos < 0)
                        rotaryPos = 2;

                    LCD_highlight(Splash, splashScreen, rotaryPos);
                }

                if(swFlag){                     // Rotary switch pressed
					swFlag  = false;
                    refresh = true;

                    if(rotaryPos == 5)
                        screen = Splash;
                    else
                        screen  = (menu_choice) rotaryPos;

                    rotaryPos = 0;
				}

				break;

            case Menu:

                //if(printLCDOnce){
                    //printLCDOnce = false;
                LCD_header(menuScreen);
                ST7735_DrawFastHLine(0, 18, 130, ST7735_WHITE);

                LCD_body(menuScreen, rotaryPos);

                ST7735_DrawFastHLine(0, 145, 130, ST7735_WHITE);
                LCD_footer(menuScreen);
                //}

                if(cwFlag){                 // Clockwise selection
                    cwFlag = false;
                    rotaryPos++;

                    if(rotaryPos > 5)
                        rotaryPos = 0;

                    LCD_highlight(Menu, menuScreen, rotaryPos);
                }else if(ccwFlag){          // Counter-Clockwise selection
                    ccwFlag = false;
                    rotaryPos--;

                    if(rotaryPos < 0)
                        rotaryPos = 4;

                    LCD_highlight(Menu, menuScreen, rotaryPos);
                }

                if(swFlag){
                    swFlag  = false;
                    refresh = true;

                    if(rotaryPos == 5)
                        screen = Splash;
                    else
                        screen  = (menu_choice) rotaryPos;

                    rotaryPos = 0;
                }

                break;

			case Settings:

				LCD_header(settingScreen);
				ST7735_DrawFastHLine(0, 18, 130, ST7735_WHITE);

				LCD_body(settingScreen, rotaryPos);

				ST7735_DrawFastHLine(0, 145, 130, ST7735_WHITE);
                LCD_footer(settingScreen);

                if(cwFlag){                 // Clockwise selection
                    cwFlag = false;
                    rotaryPos++;

                    if(rotaryPos < 5)
                        rotaryPos = 0;

                    //LCD_highlight(Settings, settingScreen, rotaryPos);
                }else if(ccwFlag){          // Counter-Clockwise selection
                    ccwFlag = false;
                    rotaryPos--;

                    if(rotaryPos < 0)
                        rotaryPos = 4;

                    //LCD_highlight(Settings, settingScreen, rotaryPos);
                }

                if(swFlag){
					swFlag  = false;
                    refresh = true;

                    if(rotaryPos == 5)
                        screen = Splash;
                    else
                        screen  = (menu_choice) rotaryPos;

                    rotaryPos = 0;
				}

				break;

			default: screen = Splash; break;
		}
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
		readRTC.temperature_mode = writeRTC.temperature_mode;

    } while(error);
}

/* TO-DO: Reads time from the EEPROM */
void read_EEP(void){ /*...*/ }

void turnSignal_Setup(){

    // WDT_Reset();
    TURN_SIG->SEL0 &=~ TURN_BLINK;
    TURN_SIG->SEL1 &=~ TURN_BLINK;
    TURN_SIG->DIR  |=  TURN_BLINK;
}

 void turn_Signals(void){
    
    TURN_SIG->OUT ^= TURN_BLINK;
    Timer32_msdelay(TURN_SIG_HALFSEC);
 }


/********************* INTERRUPT SERVICE ROUTINES ****************************/

 /* Hall Effect Interrupt */
 void PORT2_IRQHandler(void){

     /* Store the time between hall pulse */
     if(GPIO->IFG & HALL){
         hall_flag = 1;
         cur[countVar % 5] = TIMER32_1->VALUE / ((float) TIMER32_LOAD_0); //get current val of timer
         countVar++;

         averageRPS = (cur[0] + cur[1] + cur[2] + cur[3] + cur[4]) / 5.0;
         averageRPM = (averageRPS == 0)? 0.0 : (60.0 / averageRPS);
         averageMPH = (averageRPM * 60.0 * 12.0 * 3.14) / 5280.0;
     }

     GPIO->IFG = 0;
 }

/* Adjusted SysTick Handler ~0.69 second, nice */
void SysTick_Handler(void){
    
    SysTick_count++;

    if(SysTick_count == 2){
        SysTick_count = 0; 
        read_data = 1; //flag to print every sec
        actualIN = ((period_irq * 0.33333) / 148.0);
        actualCM = 2.54 * actualIN; // FIXME: This might need to change to detect actual centimeters
    }

}

/* Timer32 1 second timer 1 */
void T32_INT1_IRQHandler(void){

    // Speedometer
    // Tachometer
    // Odometer

    //odometer_DataWrite(odoReading, miles);

    TIMER32_1->INTCLR = 0;      // Clear interrupt flag
}

/* Timer32 1 second timer 2 */
//void T32_INT2_IRQHandler(void){
//
//
//    TIMER32_2->INTCLR = 0;      // Clear interrupt flag
//}


/* Proximity Sensor TimerA Interrupt*/ 
void TA0_N_IRQHandler(void){

    int last_edge = 0;

    if(last_edge == 1)
        TIMER_A0->CTL |= TIMER_A_CTL_CLR; //clear flag
    else
        period_irq = TIMER_A0->CCR[2]; //get current value

    last_edge = !last_edge;

    TIMER_A0->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG); //clear flag
}

/*****************************************************************************/
