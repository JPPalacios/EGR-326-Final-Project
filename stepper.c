#include "msp.h"
#include "stepper.h"
#include "Timers.h"

/* Bipolar Stepper Motor Setup */
void stepper_Setup(void){

    // Tachometer and Speedometer
    BIPOLAR->SEL0 &=~ (TACHO | SPEEDO); 
    BIPOLAR->SEL1 &=~ (TACHO | SPEEDO);
    BIPOLAR->DIR  |=  (TACHO | SPEEDO);
    BIPOLAR->OUT  &=~ (TACHO | SPEEDO);
}

void run_Speedometer(int steps, int direction){
    int count = 0; 
    bipolar_state = speedHeldState;

    while(1){
        WDT_Reset();

        if(steps <= 80000){
            switch(bipolar_state){
                case Step1: 
                    BIPOLAR->OUT |=  SINT_1;
                    BIPOLAR->OUT |=  SINT_2;
                    BIPOLAR->OUT |=  SINT_3;
                    BIPOLAR->OUT &=~ SINT_4;

                    Timer32_usdelay(SPEED);
                    count++;

                    bipolar_state = (direction == 0) ? Step2 : Step4; 
                    break;
                case Step2:
                    BIPOLAR->OUT &=~ SINT_1;
                    BIPOLAR->OUT |=  SINT_2;
                    BIPOLAR->OUT |=  SINT_3;
                    BIPOLAR->OUT |=  SINT_4;

                    Timer32_usdelay(SPEED);
                    count++; 

                    bipolar_state = (direction == 0) ? Step3 : Step1; 
                    break;
                case Step3:
                    BIPOLAR->OUT |=  SINT_1;
                    BIPOLAR->OUT &=~ SINT_2;
                    BIPOLAR->OUT |=  SINT_3;
                    BIPOLAR->OUT |=  SINT_4;

                    Timer32_usdelay(SPEED);
                    count++;  

                    bipolar_state = (direction == 0) ? Step4 : Step2; 
                    break;
                case Step4: 
                    BIPOLAR->OUT |=  SINT_1;
                    BIPOLAR->OUT |=  SINT_2;
                    BIPOLAR->OUT &=~ SINT_3;
                    BIPOLAR->OUT |=  SINT_4;

                    Timer32_usdelay(SPEED);
                    count++;  

                    bipolar_state = (direction == 0) ? Step1 : Step3; 
                    break;            
                default: 
                    if(count >= steps){
                        speedHeldState = bipolar_state; 
                        break;
                    }
                    bipolar_state = Step1; 
            }
        }else break;

        if(count >= steps) break; 

    }
} 

void run_Tachometer(int steps, int direction){
    int count = 0;
    bipolar_state = speedHeldState;

    while(1){
        WDT_Reset();

        if(steps <= 80000){
            switch(bipolar_state){
                case Step1:
                    BIPOLAR->OUT |=  TINT_1;
                    BIPOLAR->OUT |=  TINT_2;
                    BIPOLAR->OUT &=~ TINT_3;
                    BIPOLAR->OUT &=~ TINT_4;

                    Timer32_usdelay(SPEED);
                    count++;

                    bipolar_state = (direction == 0) ? Step2 : Step4;
                    break;
                case Step2:
                    BIPOLAR->OUT &=~ TINT_1;
                    BIPOLAR->OUT |=  TINT_2;
                    BIPOLAR->OUT |=  TINT_3;
                    BIPOLAR->OUT &=~ TINT_4;

                    Timer32_usdelay(SPEED);
                    count++;

                    bipolar_state = (direction == 0) ? Step3 : Step1;
                    break;
                case Step3:
                    BIPOLAR->OUT &=~ TINT_1;
                    BIPOLAR->OUT &=~ TINT_2;
                    BIPOLAR->OUT |=  TINT_3;
                    BIPOLAR->OUT |=  TINT_4;

                    Timer32_usdelay(SPEED);
                    count++;

                    bipolar_state = (direction == 0) ? Step4 : Step2;
                    break;
                case Step4:
                    BIPOLAR->OUT |=  TINT_1;
                    BIPOLAR->OUT &=~ TINT_2;
                    BIPOLAR->OUT &=~ TINT_3;
                    BIPOLAR->OUT |=  TINT_4;

                    Timer32_usdelay(SPEED);
                    count++;

                    bipolar_state = (direction == 0) ? Step1 : Step3;
                    break;
                default:
                    if(count >= steps){
                        speedHeldState = bipolar_state;
                        break;
                    }
                    bipolar_state = Step1;
            }
        }else break;

        if(count >= steps) break;

    }
}
