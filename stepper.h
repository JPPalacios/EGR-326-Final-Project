#ifndef __STEPPER_H__
#define __STEPPER_H__

#include "msp.h"
#include "ST7735.h"


// SPEED SENSOR
#define GPIO P2
#define HALL BIT3
#define SPEED_ARR_SIZE 6

// TACHOMETER MOTOR
#define BIPOLAR P5
#define TINT_1     BIT0
#define TINT_2     BIT1
#define TINT_3     BIT2
#define TINT_4     BIT3
#define SPEED 200   // 500, 200
#define MAXT  500

// SPEEDOMETER MOTOR
#define SINT_1     BIT4
#define SINT_2     BIT5
#define SINT_3     BIT6
#define SINT_4     BIT7
#define TACHO  (BIT3 | BIT2 | BIT1 | BIT0)
#define SPEEDO (BIT7 | BIT6 | BIT5 | BIT4)
#define MAXS  520

#define ODO      P10
#define ODO_DATA BIT2
#define ODO_CS   BIT0
#define ODO_CLK  BIT1
#define ODO_BTN  BIT4

volatile int count;
volatile uint8_t direction;
volatile int steps, currSteps, prevSteps, tachoSteps;
volatile int hall_flag;
volatile float averageRPS, rollingSum;
volatile float averageRPM;
volatile float averageMPH;
volatile int countVar;
volatile float cur[5];
//uint8_t i = 0, j = 0;

#define TEST_ODOMETER_STATE Mileage
float speedPos, temPos, speed;

typedef enum {
    Step1, 
    Step2,
    Step3, 
    Step4
} bipolar_state; 

typedef enum{   // State of the odometer controlled by the odometer button
    Reset,
    Mileage,
    TripA,
    TripB
} odometer_state;

int speedHeldState, tempHeldState; 

// Odometer Variables
volatile int miles;
bipolar_state state;

void hall_Setup(void);

void stepper_Setup(void); 
void run_Speedometer(int steps, int direction); 
void run_Tachometer(int steps);

void odometer_Setup(void);
void odometer_DataWrite(odometer_state odoReading, int miles);
void SPI_Write(uint8_t address, uint8_t data);

#endif 
