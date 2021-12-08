#include "msp.h"
#include "stdio.h"
#include "Proximity.h"

/* ---------- Timer A Library ---------- */
void TimerA0_Setup(void){

    PROX->SEL0 |=  (PROX_LED | ECHO);
    PROX->SEL1 &= ~(PROX_LED | ECHO);
    PROX->DIR  |=  PROX_LED;            // Compare Mode
    PROX->DIR  &= ~ECHO;                // Capture Mode

    // PROX->SEL0 &=~ SOUND;            // FIXME: ADD SPEAKERS TO THE SETUP
    // PROX->SEL1 &=~ SOUND;
    // PROX->DIR  |=  SOUND;
    // PROX->REN  |=  SOUND;
    // PROX->OUT  &=~ SOUND;

    // Port 2.5 - Timer A0.3 (Capture Mode -> Measure Intervals Echo)
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_CM_3   | // Capture on both edges,
            TIMER_A_CCTLN_CCIS_0 | // Use CCI2A,
            TIMER_A_CCTLN_CCIE   | // Enable capture interrupt,
            TIMER_A_CCTLN_CAP    | // Enable capture mode,
            TIMER_A_CCTLN_SCS;     // Synchronous capture

    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |    // Use SMCLK as clock source,
            TIMER_A_CTL_MC_2     |             // Start timer in continuous mode
            TIMER_A_CTL_CLR;                   // clear TA0R = 0x0224

    // Timer A0.1 (Compare Mode) -> LED Intensity:
    TIMER_A0->CCR[0]= 60000;                        // CCR0 PWM period for LED
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     // CCTL reset/set //was 7//was2
}

void TimerA3_Setup(void){

    // Port 10.4 - Timer A3.0 (Compare Mode -> Proximity PWM Trig)
    PROX_TRIG->SEL0 |= TRIG;
    PROX_TRIG->SEL1 &= ~TRIG;
    PROX_TRIG->DIR  |= TRIG;

    TIMER_A3->CCR[0] = 60000 - 1; //period
    TIMER_A3->CCR[1] = 9 - 1; //DC
    TIMER_A3->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //reset/set
    TIMER_A3->CTL = TIMER_A_CTL_TASSEL_2    |       // Use SMCLK as clock source,
            TIMER_A_CTL_MC_1                |       // Start timer in up mode
            TIMER_A_CTL_ID__4               |       // div by 8
            TIMER_A_CTL_CLR;                        // clear TA0R

    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);
    __enable_irq();

}

void read_Proximity(void){

    // FIXME: ADD WATCHDOG TIMER HERE

    if(read_data){
        read_data = 0; //reset print flag
        // printf("Distance: %f inches\n", actual); //print actual distance

        if (actualIN <= 8){
            TIMER_A0->CCR[0] =  3000;          // CCR1 PWM period
            PROX->OUT  |= SOUND;    // FIXME: ADD A SPEAKER ON TIMER A
            __delay_cycles(10000);  // FIXME: CHANGE THIS TIMER
            PROX->OUT  &=~ SOUND;
        }

        else if(actualIN > 8)
            TIMER_A0->CCR[0] = 0;              // CCR1 PWM period
    }
    
}

