#include "msp.h"
#include "encoder.h"
#include "stdio.h"
#include "stdbool.h"

bool ccwFlag = false, cwFlag = false, swFlag = false;

void encoder_Setup(void){

    ENCODER->SEL0 &=~ ENC_SETUP;
    ENCODER->SEL1 &=~ ENC_SETUP;
    ENCODER->DIR  &=~ ENC_SETUP;  // Inputs
    ENCODER->REN  |=  ENC_SETUP;
    ENCODER->OUT  |=  ENC_SETUP;  // Data Output Register -> Pull-up
    ENCODER->IE   |=  ENC_SETUP;  // Interrupt Enable Register
    ENCODER->IES  &=~ ENC_SETUP;  // Interrupt Edge Select Register
    ENCODER->IFG  &=~ ENC_SETUP;  // Clear Interrupt Flags

    NVIC->ISER[1] = 1 << ((PORT3_IRQn) & 31); // Enabled interrupts for pushbutton detection
    __enable_interrupt();
}

void PORT3_IRQHandler(void){

    if(ENCODER->IFG & ENC_SW)
        swFlag = true;

    if(ENCODER->IFG & ENC_CLK){
         if(ENCODER->IES & ENC_CLK){  // rising edge
            if(ENCODER->IN & ENC_DT)
                cwFlag = true;
            else if(!cwFlag)
                ccwFlag = true;
        }else if(!(ENCODER->IES & ENC_CLK)){ // falling edge
            if(ENCODER->IN & ENC_DT)
                ccwFlag = true;
            else if(!ccwFlag)
                cwFlag = true;
        }
    }

    ENCODER->IES ^= ENC_CLK; // switch the edge to read (rise or fall)

    ENCODER->IFG &=~ ENC_SETUP;
}
