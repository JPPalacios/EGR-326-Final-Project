#include "msp.h"
#include "stdio.h"
#define ROTARY P3
#define CLK    BIT7
#define DT     BIT6
#define SW     BIT5

#define ROTARY_SETUP (CLK | DT | SW)

void Rotary_setup();

volatile int SW_flag, CCWcount, CWcount;

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	Rotary_setup();

	while(1){
	    if(SW_flag){
	        SW_flag = 0;
	        CWcount = 0;
	        CCWcount = 0;
	    }
	    __delay_cycles(3000*1000);
	    printf("CW: %d\tCCW: %d\n", CWcount, CCWcount);
	}

}

void Rotary_setup(){

    // Rotary Encoder Setup
    ROTARY->SEL0 &=~ ROTARY_SETUP;
    ROTARY->SEL1 &=~ ROTARY_SETUP;
    ROTARY->DIR  &=~ ROTARY_SETUP;  // Inputs
    ROTARY->REN  |=  ROTARY_SETUP;
    ROTARY->OUT  |=  ROTARY_SETUP;  // Data Output Register -> Pull-up
    ROTARY->IE   |=  ROTARY_SETUP;  // Interrupt Enable Register
    ROTARY->IES  &=~ ROTARY_SETUP;  // Interrupt Edge Select Register
    ROTARY->IFG  &=~ ROTARY_SETUP;  // Clear Interrupt Flags

    NVIC->ISER[1] = 1 << ((PORT3_IRQn) & 31); // Enabled interrupts for pushbutton detection
    __enable_interrupt();
}

void PORT3_IRQHandler(void){

    if(ROTARY->IFG & SW)
        SW_flag = 1;

    if(ROTARY->IFG & CLK){
         if(ROTARY->IES & CLK){  // rising edge
            if(ROTARY->IN & DT)
                CWcount++;
            else
                CCWcount++;
        }else if(!(ROTARY->IES & CLK)){ // falling edge
            if(ROTARY->IN & DT)
                CCWcount++;
            else
                CWcount++;
        }
    }

    ROTARY->IES ^= CLK; // switch the edge to read (rise or fall)

    ROTARY->IFG &=~ ROTARY_SETUP;
}
