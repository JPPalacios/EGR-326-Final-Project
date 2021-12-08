#ifndef __STEPPER_H__
#define __STEPPER_H__

#include "msp.h"

#define BIPOLAR    P5
#define TINT_1     BIT0
#define TINT_2     BIT1
#define TINT_3     BIT2
#define TINT_4     BIT3

#define SINT_1     BIT4
#define SINT_2     BIT5
#define SINT_3     BIT6
#define SINT_4     BIT7
#define TACHO  (BIT3 | BIT2 | BIT1 | BIT0)
#define SPEEDO (BIT7 | BIT6 | BIT5 | BIT4)

#define SPEED 10
#define MAXS  520
#define MAXT  12

float speedPos, temPos, speed;

enum states {
    Step1, 
    Step2,
    Step3, 
    Step4
} bipolar_state; 

int speedHeldState, tempHeldState; 

void stepper_Setup(void); 
void run_Speedometer(int steps, int direction); 
void run_Tachometer(int steps, int direction); 

#endif 
