#include "msp.h"
#include "stdio.h"
#include "stdio.h"
#include "SysTick.h"
#include "bipolar_motor.h"

#define ODO P10
#define BUTTON BIT1

void tachometer_setup(); //odometer trip

int i, j, cw_flag, ccw_flag, dirSelect;
int cycles, direction;

volatile int TX_mode = 1; // 0 -> Slave Receive Mode, 1 -> Master Transmit Mode

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    SysTick_setup();
    tripButton_setup();
    tachometer_setup();

    __enable_irq(); // All interrupts are enabled

    while(1){

        printf("Enter cycles: ");
        scanf("%d", &cycles);

        printf("\nEnter direction: CW - 1 | CCW - 0 \n\n");
        scanf("%d", &dirSelect);

        if(dirSelect == 1){
            cw_flag = 1;
            ccw_flag = 0;
        }

        else if(dirSelect == 0){
            cw_flag = 0;
            ccw_flag = 1;
        }

        while(!cw_flag && !ccw_flag);

        if(cw_flag && !ccw_flag){
            ccw_flag = 0; cw_flag = 0;
            printf("Motor Turns: Clockwise %d cycles\n\n", cycles);
            count++; //increment thru state machine to turn clockwise
        }
        else if(ccw_flag && !cw_flag){
            ccw_flag = 0; cw_flag = 0;
            printf("Motor Turns: Counterclockwise %d cycles\n\n", cycles);
            count--; //decrement thru state machine to turn counterclockwise
        }

        do{
        tachometer_logic(); //state machine for tachometer
        } while(cycles >= logicCycle); //cycle thru logic for as many cycles as defined

        logicCycle = 0;
    }
}


void tripButton_setup(){ //pushbutton - use for 7 seg odometer integration

    ODO->SEL0 &= ~BUTTON;
    ODO->SEL1 &= ~BUTTON;
    ODO->DIR  &= ~BUTTON;
    ODO->REN  |=  BUTTON;
    ODO->OUT  |=  BUTTON;
    ODO->IE   |=  BUTTON;
    ODO->IES  &= ~BUTTON;
    ODO->IFG  = 0;

    NVIC->ISER[1] = 1 << ((PORT3_IRQn) & 31); // Enabled interrupts for pushbutton detection
}

void PORT10_IRQHandler(){

    //interrupt for total mileage, trip and trip reset

    ODO->IFG = 0;
}

